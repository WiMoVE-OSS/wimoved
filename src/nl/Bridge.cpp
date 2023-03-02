#include "Bridge.h"

#include <stdexcept>

nl::Bridge::Bridge() {
    link = rtnl_link_bridge_alloc();
    if (link == nullptr) {
        throw std::runtime_error("rtnl_link_bridge: could not allocate");
    }
}

nl::Bridge::~Bridge() { rtnl_link_put(link); }
