#include <cstring>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
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

IPCSocket::IPCSocket(const std::string& iface) : local{AF_UNIX, "\0"}, dest() {
    std::string socket_path  = "/var/run/hostapd/" + iface;

    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        throw std::runtime_error(std::string("could not create socket: ") + std::strerror(errno));
    }

    // TODO: handle errno = EADDRINUSE
    int err = bind(sock_fd, (struct sockaddr *) &local, sizeof(local)) < 0;
    if (err == -1) {
        throw std::runtime_error(std::string("could not bind to socket: ") + std::strerror(errno));
    }

    dest.sun_family = AF_UNIX;
    strncpy(dest.sun_path, socket_path.c_str(), sizeof(local.sun_path));
    local.sun_path[sizeof(local.sun_path)-1] = '\0';
    err = connect(sock_fd, (struct sockaddr *) &dest, sizeof(dest));
    if (err == -1) {
        close(sock_fd);
        throw std::runtime_error(std::string("could not connect to socket: ") + std::strerror(errno));
    }
}

IPCSocket::~IPCSocket() {
    if (close(sock_fd) == -1) {
        std::cerr << "could not close socket: " << std::strerror(errno) << "\n";
    }
}

std::string IPCSocket::send_command(const std::vector<std::string>& args) const {
    std::string command = join(args, ' ');

    ssize_t err = send(sock_fd, command.c_str(), command.size(), 0);
    if (err < 0) {
        throw std::runtime_error(std::string("could not send to socket: ") + std::strerror(errno));
    }

    std::string buf(4096, ' ');
    ssize_t len = recv(sock_fd, &buf[0], buf.size(), 0);
    if (len == -1) {
        throw std::runtime_error(std::string("could not recv from socket: ") + std::strerror(errno));
    }
    return buf.substr(0, len);
}