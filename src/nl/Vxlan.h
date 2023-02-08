#ifndef GAFFA_VXLAN_H
#define GAFFA_VXLAN_H

#include <netlink/route/link/vxlan.h>

namespace nl {
    class Vxlan {
    public:
        Vxlan();
        Vxlan(const Vxlan &other) = delete;
        ~Vxlan();

        rtnl_link *link;
    };
}

#endif //GAFFA_VXLAN_H
