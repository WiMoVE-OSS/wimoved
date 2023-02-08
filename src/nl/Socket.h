#ifndef GAFFA_NL_SOCKET_H
#define GAFFA_NL_SOCKET_H

#include <netlink/netlink.h>

namespace nl {
    class Socket {
    public:
        Socket();
        ~Socket();
        void create_vxlan_iface(uint32_t vni);
    private:
        struct nl_sock *socket;
    };
}


#endif //GAFFA_NL_SOCKET_H
