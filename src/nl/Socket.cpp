#include "Socket.h"

#include <net/if.h>
#include <netlink/cache.h>
#include <netlink/netlink.h>
#include <netlink/route/link/vxlan.h>

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "Bridge.h"
#include "Link.h"
#include "Vxlan.h"
#include "logging/loginit.h"

const std::string VXLAN_IFACE_PREFIX = "vxlan";
const std::string BRIDGE_IFACE_PREFIX = "bridge";
const int VXLAN_PORT = 4789;

nl::Socket::Socket() : socket(nl_socket_alloc()) {
    if (socket == nullptr) {
        throw std::runtime_error(std::string("error in nl_socket_alloc: ") + std::strerror(errno));
    }
    int err = nl_connect(socket, NETLINK_ROUTE);
    if (err < 0) {
        throw std::runtime_error(std::string("error in nl_connect: ") + nl_geterror(err));
    }
}

nl::Socket::~Socket() {
    nl_close(socket);
    nl_socket_free(socket);
}

void nl::Socket::create_vxlan_iface(uint32_t vni) {
    Vxlan vxlan;

    rtnl_link_set_name(vxlan.link, (VXLAN_IFACE_PREFIX + std::to_string(vni)).c_str());
    rtnl_link_set_flags(vxlan.link, IFF_UP);

    if (rtnl_link_vxlan_set_id(vxlan.link, vni) < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_id: ") + std::strerror(errno));
    }

    int err = rtnl_link_vxlan_set_port(vxlan.link, VXLAN_PORT);
    if (err < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_port_range: ") + nl_geterror(err));
    }
    // TODO: properly handle VXLAN iface exists
    err = rtnl_link_add(socket, vxlan.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            WMLOG(WARNING) << "rtnl_link_add: vxlan interface with vni " << vni << " already exists";
        } else {
            throw std::runtime_error(std::string("error in rtnl_link_add: ") + nl_geterror(err));
        }
    }
    WMLOG(INFO) << "Created VXLAN " << vni;
}

void nl::Socket::create_bridge_for_vni(uint32_t vni) { create_bridge(BRIDGE_IFACE_PREFIX + std::to_string(vni)); }

void nl::Socket::create_bridge(const std::string &name) {
    Bridge bridge;
    rtnl_link_set_name(bridge.link, name.c_str());
    rtnl_link_set_flags(bridge.link, IFF_UP);

    int err = rtnl_link_add(socket, bridge.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            WMLOG(WARNING) << "rtnl_link_add: bridge interface with name " << name << " already exists";
        } else {
            throw std::runtime_error(std::string("error in rtnl_link_add: ") + nl_geterror(err));
        }
    }

    WMLOG(INFO) << "Created Bridge " << name;
}

void nl::Socket::add_iface_bridge(const std::string &bridge_name, const std::string &interface_name) {
    Bridge bridge;
    Link link;
    int err = rtnl_link_get_kernel(socket, 0, interface_name.c_str(), &link.link);
    if (err < 0) {
        throw std::runtime_error("Could not get interface: " + interface_name + " " + nl_geterror(err));
    }
    err = rtnl_link_get_kernel(socket, 0, bridge_name.c_str(), &bridge.link);
    if (err < 0) {
        throw std::runtime_error("Could not get interface: " + bridge_name + " " + nl_geterror(err));
    }
    err = rtnl_link_enslave(socket, bridge.link, link.link);
    if (err < 0) {
        throw std::runtime_error("Could not enslave interface: " + interface_name + " to Bridge " + bridge_name + " " +
                                 nl_geterror(err));
    }
    WMLOG(INFO) << "Connected " << interface_name << " to " << bridge_name;
}

void nl::Socket::delete_interface(const std::string &name) {
    Link link;

    rtnl_link_set_name(link.link, name.c_str());

    int err = rtnl_link_delete(socket, link.link);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not delete link: ") + nl_geterror(err));
    }
    WMLOG(INFO) << "Deleted " << name;
}

std::unordered_set<uint32_t> nl::Socket::interface_list() {
    struct nl_cache *cache = nullptr;
    int err = rtnl_link_alloc_cache(socket, AF_INET, &cache);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not allocate cache: ") + nl_geterror(err));
    }
    struct nl_object *object = nl_cache_get_first(cache);
    std::unordered_set<uint32_t> set_of_vnis(0);
    do {
        auto *link = reinterpret_cast<struct rtnl_link *>(object);
        if (rtnl_link_is_vxlan(link) != 0) {
            uint32_t id = 0;
            err = rtnl_link_vxlan_get_id(link, &id);
            if (err < 0) {
                throw std::runtime_error(std::string("Could not get vni ") + nl_geterror(err));
            }
            set_of_vnis.emplace(id);
        }
    } while ((object = nl_cache_get_next(object)) != nullptr);
    return set_of_vnis;
}
