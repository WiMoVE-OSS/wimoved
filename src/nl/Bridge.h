#ifndef WM_BRIDGE_H
#define WM_BRIDGE_H

#include <netlink/route/link/bridge.h>

namespace nl {
class Bridge {
   public:
    Bridge();
    Bridge(const Bridge &other) = delete;
    ~Bridge();

    rtnl_link *link;
};
}  // namespace nl

#endif  // WM_BRIDGE_H
