#include "Vxlan.h"

#include <stdexcept>

nl::Vxlan::Vxlan() : link(rtnl_link_vxlan_alloc()) {
    if (link == nullptr) {
        throw std::runtime_error("rtnl_link_vxlan_alloc: could not allocate");
    }
}

nl::Vxlan::~Vxlan() { rtnl_link_put(link); }
