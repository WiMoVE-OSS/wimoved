#include <cstring>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <optional>
#include "IPCSocket.h"

std::string join(const std::vector<std::string>& strings, char separator) {
    std::ostringstream o;
    auto it = strings.begin();
    if (it != strings.end())
    {
        for(o << *it++; it != strings.end(); ++it)
            o << separator << *it;
    }
    return o.str();
}

IPCSocket::IPCSocket(const std::string& iface, const std::chrono::duration<int>& timeout) : local{AF_UNIX, "\0"}, dest() {
    std::string socket_path  = "/var/run/hostapd/" + iface;

    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        throw std::runtime_error(std::string("could not create socket: ") + std::strerror(errno));
    }
    auto timeout_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    struct timeval tv{
            timeout_usec.count() / 1000000,
            timeout_usec.count() % 1000000,
    };
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    // TODO: handle errno = EADDRINUSE
    if (bind(sock_fd, (struct sockaddr *) &local, sizeof(local)) == -1) {
        close(sock_fd);
        throw std::runtime_error(std::string("could not bind to socket: ") + std::strerror(errno));
    }

    dest.sun_family = AF_UNIX;
    strncpy(dest.sun_path, socket_path.c_str(), sizeof(local.sun_path));
    local.sun_path[sizeof(local.sun_path)-1] = '\0';
    if (connect(sock_fd, (struct sockaddr *) &dest, sizeof(dest)) == -1) {
        close(sock_fd);
        throw std::runtime_error(std::string("could not connect to socket: ") + std::strerror(errno));
    }
}

IPCSocket::~IPCSocket() {
    if (close(sock_fd) == -1) {
        std::cerr << "could not close socket: " << std::strerror(errno) << "\n";
    }
}

void IPCSocket::send_command(const std::vector<std::string> &args) const {
    std::string command = join(args, ' ');

    ssize_t err = send(sock_fd, command.c_str(), command.size(), 0);
    if (err < 0) {
        throw std::runtime_error(std::string("could not send_command to socket: ") + std::strerror(errno));
    }
}

std::optional<std::string> IPCSocket::send_and_receive(const std::vector<std::string>& args) const {
    send_command(args);
    return receive();
}

std::optional<std::string> IPCSocket::receive() const {
    std::string buf(4096, ' ');
    ssize_t len = recv(sock_fd, &buf[0], buf.size(), 0);
    if (len == -1) {
        if (errno == EAGAIN) {
            return std::nullopt;
        }
        throw std::runtime_error(std::string("could not recv from socket: ") + std::strerror(errno));
    }
    return buf.substr(0, len);
}