#ifndef GAFFA_NL_SOCKET_H
#define GAFFA_NL_SOCKET_H

#include <netlink/netlink.h>

namespace nl {
    class Socket {
    public:
        Socket();
        ~Socket();
        void create_vxlan_iface(uint32_t vni);
        void add_iface_bridge(const std::string& bridgeName, const std::string& ifaceName);
        void create_bridge_for_vni(uint32_t vni);
        void create_bridge(std::string name);
        void delete_interface(std::string name);
    private:
        struct nl_sock *socket;
    };
}


#endif //GAFFA_NL_SOCKET_H
