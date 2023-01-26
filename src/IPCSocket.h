#include <string>
#include <vector>
#include "hostapd.h"

class IPCSocket {
public:
    explicit IPCSocket(const std::string& iface);
    void send_command(const std::vector<std::string>& args);
    ~IPCSocket();
private:
    wpa_ctrl *ctrl_iface;
};