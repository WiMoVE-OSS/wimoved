#ifndef GAFFA_BRIDGE_H
#define GAFFA_BRIDGE_H

#include <netlink/route/link/bridge.h>

namespace nl {
    class Bridge {
    public:
        Bridge();
        Bridge(const Bridge &other) = delete;
        ~Bridge();

        rtnl_link *link;
    };
}

#endif //GAFFA_BRIDGE_H
