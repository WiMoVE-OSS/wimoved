#ifndef GAFFA_NL_SOCKET_H
#define GAFFA_NL_SOCKET_H

#include <netlink/list.h>
#include <netlink/netlink.h>

#include <unordered_set>
#include <vector>

#include "../SynchronizedQueue.h"

namespace nl {
class Socket {
   public:
    Socket();
    ~Socket();

    void create_vxlan_iface(uint32_t vni);
    void add_iface_bridge(const std::string& bridgeName, const std::string& ifaceName);
    void create_bridge_for_vni(uint32_t vni);
    void create_bridge(const std::string& name);
    void delete_interface(const std::string& name);
    std::unordered_set<uint32_t> interface_list();
   private:
    struct nl_sock* socket;
};
}  // namespace nl

#endif  // GAFFA_NL_SOCKET_H
