#include "Socket.h"

#include <net/if.h>
#include <netlink/cache.h>
#include <netlink/netlink.h>
#include <netlink/route/link/bridge.h>
#include <netlink/route/link/vxlan.h>

#include <cstring>
#include <stdexcept>

#include "Cache.h"
#include "Link.h"
#include "logging/loginit.h"

const std::string VXLAN_IFACE_PREFIX = "vxlan";
const std::string BRIDGE_IFACE_PREFIX = "bridge";
const int VXLAN_PORT = 4789;

nl::Socket::Socket() : socket(nl_socket_alloc()) {
    if (socket == nullptr) {
        throw std::runtime_error(std::string("Error in nl_socket_alloc error=") + std::strerror(errno));
    }
    int err = nl_connect(socket, NETLINK_ROUTE);
    if (err < 0) {
        throw std::runtime_error(std::string("Error in nl_connect error=") + nl_geterror(err));
    }
}

nl::Socket::~Socket() {
    nl_close(socket);
    nl_socket_free(socket);
}

void nl::Socket::create_vxlan_iface(uint32_t vni) {
    rtnl_link *vxlan_unmanaged = rtnl_link_vxlan_alloc();
    if (vxlan_unmanaged == nullptr) {
        throw std::runtime_error(std::string("Error in rtnl_link_vxlan_alloc error=") + std::strerror(errno));
    }
    Link vxlan(vxlan_unmanaged);

    rtnl_link_set_name(vxlan.link, (VXLAN_IFACE_PREFIX + std::to_string(vni)).c_str());
    rtnl_link_set_flags(vxlan.link, IFF_UP);

    if (rtnl_link_vxlan_set_id(vxlan.link, vni) < 0) {
        throw std::runtime_error(std::string("Error in rtnl_link_vxlan_set_id error=") + std::strerror(errno));
    }

    if (rtnl_link_vxlan_set_learning(vxlan.link, 0) < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_learning: ") + std::strerror(errno));
    }

    int err = rtnl_link_vxlan_set_port(vxlan.link, VXLAN_PORT);
    if (err < 0) {
        throw std::runtime_error(std::string("Error in rtnl_link_vxlan_set_port_range error=") + nl_geterror(err));
    }
    err = rtnl_link_add(socket, vxlan.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            WMLOG(WARNING) << "Rtnl_link_add: VXLAN interface already exists vni=" << vni;
        } else {
            throw std::runtime_error(std::string("Error in rtnl_link_add error=") + nl_geterror(err));
        }
    }
    WMLOG(INFO) << "Created VXLAN with vni=" << vni;
}

void nl::Socket::create_bridge_for_vni(uint32_t vni) { create_bridge(BRIDGE_IFACE_PREFIX + std::to_string(vni)); }

void nl::Socket::create_bridge(const std::string &name) {
    rtnl_link *bridge_unmanaged = rtnl_link_bridge_alloc();
    if (bridge_unmanaged == nullptr) {
        throw std::runtime_error(std::string("Error in rtnl_link_bridge_alloc error=") + std::strerror(errno));
    }
    Link bridge(bridge_unmanaged);
    rtnl_link_set_name(bridge.link, name.c_str());
    rtnl_link_set_flags(bridge.link, IFF_UP);

    int err = rtnl_link_add(socket, bridge.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            WMLOG(WARNING) << "Rtnl_link_add: Bridge interface already exists bridge=" << name;
        } else {
            throw std::runtime_error(std::string("Error in rtnl_link_add error=") + nl_geterror(err));
        }
    }

    WMLOG(INFO) << "Created bridge=" << name;
}

void nl::Socket::add_iface_bridge(const std::string &bridge_name, const std::string &interface_name) {
    rtnl_link *interface_link_unmanaged = nullptr;
    rtnl_link *bridge_link_unmanaged = nullptr;

    int err = rtnl_link_get_kernel(socket, 0, interface_name.c_str(), &interface_link_unmanaged);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not get interface error=") + nl_geterror(err) +
                                 std::string(" iface=") + interface_name);
    }
    Link interface_link(interface_link_unmanaged);

    err = rtnl_link_get_kernel(socket, 0, bridge_name.c_str(), &bridge_link_unmanaged);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not get interface error=") + nl_geterror(err) +
                                 std::string(" bridge=") + bridge_name);
    }
    Link bridge_link(bridge_link_unmanaged);

    err = rtnl_link_enslave(socket, bridge_link.link, interface_link.link);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not enslave interface error=") + nl_geterror(err) +
                                 std::string(" bridge=") + bridge_name + std::string(" iface=") + interface_name);
    }
    WMLOG(INFO) << "Connected bridge=" << bridge_name << " iface=" << interface_name;
}

void nl::Socket::delete_interface(const std::string &name) {
    rtnl_link *link_unmanaged = rtnl_link_alloc();
    if (link_unmanaged == nullptr) {
        throw std::runtime_error(std::string("Error in rtnl_link_alloc error=") + std::strerror(errno));
    }
    Link link(link_unmanaged);

    rtnl_link_set_name(link.link, name.c_str());

    int err = rtnl_link_delete(socket, link.link);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not delete link error=") + nl_geterror(err));
    }
    WMLOG(INFO) << "Deleted iface=" << name;
}

std::unordered_set<uint32_t> nl::Socket::interface_list() {
    struct nl_cache *cache_unmanaged = nullptr;
    int err = rtnl_link_alloc_cache(socket, AF_INET, &cache_unmanaged);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not allocate cache error=") + nl_geterror(err));
    }
    Cache cache(cache_unmanaged);

    struct nl_object *object = nl_cache_get_first(cache.cache);
    std::unordered_set<uint32_t> set_of_vnis(0);
    do {
        auto *link = reinterpret_cast<struct rtnl_link *>(object);
        if (rtnl_link_is_vxlan(link) != 0) {
            uint32_t id = 0;
            err = rtnl_link_vxlan_get_id(link, &id);
            if (err < 0) {
                throw std::runtime_error(std::string("Could not get VNI error=") + nl_geterror(err));
            }
            set_of_vnis.emplace(id);
        }
    } while ((object = nl_cache_get_next(object)) != nullptr);
    return set_of_vnis;
}
