#include <cstring>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <mutex>
#include <sys/time.h>
#include <random>
#include <grp.h>
#include <sys/stat.h>
#include "Socket.h"
#include "TimeoutException.h"

static std::string join(const std::vector<std::string>& strings, char separator) {
    std::ostringstream o;
    auto it = strings.begin();
    if (it != strings.end())
    {
        for(o << *it++; it != strings.end(); ++it)
            o << separator << *it;
    }
    return o.str();
}

static std::random_device random_device;
static std::mt19937 twister(random_device());
static std::mutex twister_mutex;
static const std::string random_alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const int random_name_len = 8;

static std::string random_name() {
    std::lock_guard g(twister_mutex);

    std::string s;
    s.reserve(random_name_len);
    for (int i = 0; i < random_name_len; i++) {
        s += random_alphabet[twister() % random_alphabet.size()];
    }
    return s;
}

ipc::Socket::Socket(const std::string& iface, const std::chrono::duration<int>& timeout) : local{AF_UNIX, "\0"}, dest() {
    std::string local_path = "/var/run/gaffa." + random_name();
    if (unlink(local_path.c_str()) == 0) {
        std::cout << "successfully unlinked " << local_path << std::endl;
    }

    std::string socket_path  = "/var/run/hostapd/" + iface;

    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        throw std::runtime_error(std::string("could not create socket: ") + std::strerror(errno));
    }
    auto timeout_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    struct timeval tv{};
    tv.tv_sec = timeout_usec.count() / 1000000;
    tv.tv_usec = timeout_usec.count() % 1000000;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    // TODO: handle errno = EADDRINUSE
    strncpy(local.sun_path, local_path.c_str(), sizeof(local.sun_path));
    local.sun_path[sizeof(local.sun_path)-1] = '\0';
    if (bind(sock_fd, (struct sockaddr *) &local, sizeof(local)) == -1) {
        close(sock_fd);
        throw std::runtime_error(std::string("could not bind to socket: ") + std::strerror(errno));
    }

    #ifdef GAFFA_IPC_SOCKET_GROUP
    std::array<char, 200> buf{};
    struct group grp{};
    struct group *grp_result = nullptr;
    getgrnam_r(GAFFA_IPC_SOCKET_GROUP, &grp, &buf[0], buf.size(), &grp_result);
    if (grp_result == nullptr) {
        throw std::runtime_error(std::string("getgrnam failed: ") + std::strerror(errno));
    }
    if (chown(local_path.c_str(), -1, grp_result->gr_gid) == -1) {
        throw std::runtime_error(std::string("could not set socket group: ") + std::strerror(errno));
    }
    if (chmod(local_path.c_str(), 0775) == -1) {
        throw std::runtime_error(std::string("could not set socket permissions: ") + std::strerror(errno));
    }
    #endif

    dest.sun_family = AF_UNIX;
    strncpy(dest.sun_path, socket_path.c_str(), sizeof(dest.sun_path));
    dest.sun_path[sizeof(dest.sun_path)-1] = '\0';
    if (connect(sock_fd, (struct sockaddr *) &dest, sizeof(dest)) == -1) {
        close(sock_fd);
        throw std::runtime_error(std::string("could not connect to socket: ") + std::strerror(errno));
    }
}

ipc::Socket::~Socket() {
    if (close(sock_fd) == -1) {
        std::cerr << "could not close socket: " << std::strerror(errno) << "\n";
    }
    if (unlink(local.sun_path) != 0) {
        std::cerr << "could not unlink socket: " << std::strerror(errno) << "\n";
    };
}

void ipc::Socket::send_command(const std::vector<std::string> &args) const {
    std::string command = join(args, ' ');

    ssize_t err = send(sock_fd, command.c_str(), command.size(), 0);
    if (err < 0) {
        throw std::runtime_error(std::string("could not send_command to socket: ") + std::strerror(errno));
    }
}

std::string ipc::Socket::send_and_receive(const std::vector<std::string>& args) const {
    send_command(args);
    return receive();
}

std::string ipc::Socket::receive() const {
    std::string buf(4096, ' ');
    while (true) {
        ssize_t len = recv(sock_fd, &buf[0], buf.size(), 0);
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