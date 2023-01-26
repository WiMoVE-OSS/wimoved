#include <array>
#include <string>
#include <vector>
#include "hostapd.h"

#include <sys/socket.h>
#include <sys/un.h>

class IPCSocket {
public:
    explicit IPCSocket(const std::string& iface);
    std::string send_command(const std::vector<std::string>& args) const;
    ~IPCSocket();
private:
    //wpa_ctrl *ctrl_iface;
    int sock_fd;
    struct sockaddr_un local;
    struct sockaddr_un dest;
};