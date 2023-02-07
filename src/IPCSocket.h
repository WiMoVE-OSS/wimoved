#ifndef GAFFA_IPCSOCKET_H
#define GAFFA_IPCSOCKET_H

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "hostapd.h"

#include <sys/socket.h>
#include <sys/un.h>

class IPCSocket {
public:
    explicit IPCSocket(const std::string &iface, const std::chrono::duration<int>& timeout);

    std::optional<std::string> send_and_receive(const std::vector<std::string>& args) const;
    void send_command(const std::vector<std::string>& args) const;
    std::optional<std::string> receive() const;
    ~IPCSocket();
private:
    //wpa_ctrl *ctrl_iface;
    int sock_fd;
    struct sockaddr_un local;
    struct sockaddr_un dest;
};

#endif // GAFFA_IPCSOCKET_H
