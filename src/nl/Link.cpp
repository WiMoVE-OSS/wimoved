#include "Link.h"

#include <stdexcept>

nl::Link::Link() {
    link = rtnl_link_alloc();
    if (link == nullptr) {
        throw std::runtime_error("rtnl_link_alloc: could not allocate");
    }
}

nl::Link::~Link() { rtnl_link_put(link); }
