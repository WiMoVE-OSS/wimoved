#include "Socket.h"

#include <grp.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <mutex>
#include <random>
#include <stdexcept>

#include "../Configuration.h"
#include "../TimeoutException.h"
#include "logging/loginit.h"

const static int GETGRNAM_BUFFER_SIZE = 200;
const static int RECV_BUFFER_SIZE = 4096;
const static int LOCAL_SOCKET_PERMISSIONS = 0775;
const static int MICROSECONDS_PER_SECOND = 1000000;

static std::string join(const std::vector<std::string>& strings) {
    std::ostringstream o;
    auto it = strings.begin();
    if (it != strings.end()) {
        for (o << *it++; it != strings.end(); ++it) {
            o << ' ' << *it;
        }
    }
    return o.str();
}

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
static std::random_device random_device;
static std::mt19937 twister(random_device());
static std::mutex twister_mutex;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

static const std::string RANDOM_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const int RANDOM_NAME_LENGTH = 8;

static std::string random_name() {
    std::lock_guard g(twister_mutex);

    std::string s;
    s.reserve(RANDOM_NAME_LENGTH);
    for (int i = 0; i < RANDOM_NAME_LENGTH; i++) {
        s += RANDOM_ALPHABET[twister() % RANDOM_ALPHABET.size()];
    }
    return s;
}

ipc::Socket::Socket(const std::chrono::duration<int>& timeout, const std::string& sockname)
    : local{AF_UNIX, "\0"}, dest() {
    std::string dest_path = Configuration::get_instance().hapd_sockdir + "/" + sockname;
    std::string local_path = "/var/run/wimoved." + random_name();
    if (unlink(local_path.c_str()) == 0) {
        WMLOG(DEBUG) << "Successfully unlinked socket" << local_path;
    }

    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        throw std::runtime_error(std::string("could not create socket: ") + std::strerror(errno));
    }
    auto timeout_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    struct timeval tv {};
    tv.tv_sec = timeout_usec.count() / MICROSECONDS_PER_SECOND;
    tv.tv_usec = timeout_usec.count() % MICROSECONDS_PER_SECOND;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));

    // TODO: handle errno = EADDRINUSE
    strncpy(local.sun_path, local_path.c_str(), sizeof(local.sun_path));
    local.sun_path[sizeof(local.sun_path) - 1] = '\0';
    if (bind(sock_fd, reinterpret_cast<struct sockaddr*>(&local), sizeof(local)) == -1) {
        close(sock_fd);
        throw std::runtime_error("could not bind to socket " + dest_path + ": " + std::strerror(errno));
    }

    std::array<char, GETGRNAM_BUFFER_SIZE> buf{};
    struct group grp {};
    struct group* grp_result = nullptr;
    getgrnam_r(Configuration::get_instance().hapd_group.c_str(), &grp, buf.data(), buf.size(), &grp_result);
    if (grp_result == nullptr) {
        throw std::runtime_error(std::string("getgrnam failed: ") + std::strerror(errno));
    }
    if (chown(local_path.c_str(), -1, grp_result->gr_gid) == -1) {
        throw std::runtime_error(std::string("could not set socket group: ") + std::strerror(errno));
    }
    if (chmod(local_path.c_str(), LOCAL_SOCKET_PERMISSIONS) == -1) {
        throw std::runtime_error(std::string("could not set socket permissions: ") + std::strerror(errno));
    }

    dest.sun_family = AF_UNIX;
    strncpy(dest.sun_path, dest_path.c_str(), sizeof(dest.sun_path));
    dest.sun_path[sizeof(dest.sun_path) - 1] = '\0';
    if (connect(sock_fd, reinterpret_cast<struct sockaddr*>(&dest), sizeof(dest)) == -1) {
        close(sock_fd);
        throw std::runtime_error("could not connect to socket at " + dest_path + " : " + std::strerror(errno));
    }
}

ipc::Socket::~Socket() {
    if (sock_fd == -1) {
        return;
    }
    if (close(sock_fd) == -1) {
        WMLOG(ERROR) << "Could not close socket: " << std::strerror(errno) << "\n";
    }
    if (unlink(local.sun_path) != 0) {
        WMLOG(ERROR) << "Could not unlink socket: " << std::strerror(errno) << "\n";
    }
}

// NOLINTNEXTLINE(readability-make-member-function-const)
void ipc::Socket::send_command(const std::vector<std::string>& args) {
    std::string command = join(args);

    ssize_t err = send(sock_fd, command.c_str(), command.size(), 0);
    if (err < 0) {
        throw std::runtime_error(std::string("could not send_command to socket: ") + std::strerror(errno));
    }
}

std::string ipc::Socket::send_and_receive(const std::vector<std::string>& args) {
    send_command(args);
    return receive();
}

// NOLINTNEXTLINE(readability-make-member-function-const)
std::string ipc::Socket::receive() {
    std::string buf(RECV_BUFFER_SIZE, ' ');
    while (true) {
        ssize_t len = recv(sock_fd, buf.data(), buf.size(), 0);
        if (len != -1) {
            return buf.substr(0, len);
        }
        if (errno == EAGAIN) {
            throw TimeoutException("timeout in recv() from hostapd");
        }
        if (errno != EINTR) {
            throw std::runtime_error(std::string("could not recv from socket: ") + std::strerror(errno));
        }
    }
}
ipc::Socket::Socket(ipc::Socket&& other) noexcept : sock_fd(other.sock_fd), local(other.local), dest(other.dest) {
    other.sock_fd = -1;
}
