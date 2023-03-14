#ifndef WM_VXLAN_H
#define WM_VXLAN_H

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

#endif  // WM_VXLAN_H
