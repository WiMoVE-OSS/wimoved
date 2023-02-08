#include <stdexcept>
#include "NetlinkVxlan.h"

NetlinkVxlan::NetlinkVxlan() {
    link = rtnl_link_vxlan_alloc();
    if (link == nullptr) {
        throw std::runtime_error("rtnl_link_vxlan_alloc: could not allocate");
    }
}

NetlinkVxlan::~NetlinkVxlan() {
    rtnl_link_put(link);
}
