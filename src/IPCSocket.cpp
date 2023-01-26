#include <sstream>
#include <stdexcept>
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

IPCSocket::IPCSocket(const std::string& iface) {
    std::string socket_path  = "/var/run/hostapd/" + iface;

    this->ctrl_iface = wpa_ctrl_open(socket_path.c_str());
    if (this->ctrl_iface == nullptr) {
        throw std::runtime_error("wpa_ctrl_open failed");
    }
}

IPCSocket::~IPCSocket() {
    wpa_ctrl_close(this->ctrl_iface);
}

void IPCSocket::send_command(const std::vector<std::string>& args) {
    std::string full_command = join(args, ' ').c_str();
    wpa_ctrl_request(this->ctrl_iface, full_command.c_str(), full_command.size(), NULL, NULL, NULL);
}