#ifndef WIMOVED_NL_SOCKET_H
#define WIMOVED_NL_SOCKET_H

#include <netlink/list.h>
#include <netlink/netlink.h>

#include <unordered_set>
#include <vector>

#include "../SynchronizedQueue.h"

namespace nl {
class Socket {
   public:
    Socket();
    Socket(const Socket& other) = delete;
    Socket operator=(const Socket& other) = delete;
    Socket(Socket&& other) = delete;
    Socket operator=(Socket&& other) = delete;
    ~Socket();

    void create_vxlan_iface(uint32_t vni);
    void add_iface_bridge(const std::string& bridge_name, const std::string& interface_name);
    void create_bridge_for_vni(uint32_t vni);
    void create_bridge(const std::string& name);
    void delete_interface(const std::string& name);
    std::unordered_set<uint32_t> interface_list();

   private:
    struct nl_sock* socket;
};
}  // namespace nl

#endif  // WIMOVED_NL_SOCKET_H
