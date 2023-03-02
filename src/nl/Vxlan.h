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
}  // namespace nl

#endif  // GAFFA_VXLAN_H
