#ifndef GAFFA_NETLINKRENDERER_H
#define GAFFA_NETLINKRENDERER_H


#include <cstdlib>
#include <netlink/netlink.h>

#include <netlink/cache.h>

#include <netlink/route/link.h>
#include <netlink/errno.h>
#include <netlink/route/link/vxlan.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
class NetLinkRenderer {

public:
    void createVxlanIface(uint32_t vni);
    void deleteInterface(std::string);
    void createBridge(uint32_t vni);
};
#endif //GAFFA_NETLINKRENDERER_H




