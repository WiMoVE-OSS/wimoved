#ifndef WIMOVED_BRIDGE_H
#define WIMOVED_BRIDGE_H

#include <netlink/route/link/bridge.h>

namespace nl {
class Bridge {
   public:
    Bridge();
    Bridge(const Bridge& other) = delete;
    Bridge operator=(const Bridge& other) = delete;
    Bridge(Bridge&& other) = delete;
    Bridge operator=(Bridge&& other) = delete;
    ~Bridge();

    rtnl_link* link;
};
}  // namespace nl

#endif  // WIMOVED_BRIDGE_H
