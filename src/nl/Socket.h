#ifndef GAFFA_NL_SOCKET_H
#define GAFFA_NL_SOCKET_H

#include <netlink/netlink.h>
#include <netlink/list.h>
#include <unordered_set>
#include <vector>
#include "Event.h"
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

        void subscribe();
        void set_receive_timeout(const std::chrono::duration<int> &timeout);
        std::vector<Event> wait_for_events();
        std::vector<nl::Event> events;
    private:
        struct nl_sock *socket;
    };
}


#endif //GAFFA_NL_SOCKET_H
