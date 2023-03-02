#ifndef GAFFA_NETLINKRENDERER_H
#define GAFFA_NETLINKRENDERER_H

#include <netlink/cache.h>
#include <netlink/errno.h>
#include <netlink/netlink.h>
#include <netlink/route/link.h>
#include <netlink/route/link/vxlan.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
class NetLinkRenderer {
   public:
    void createVxlanIface(uint32_t vni);
    void deleteInterface(std::string);
    void createBridge(uint32_t vni);
};
#endif  // GAFFA_NETLINKRENDERER_H
