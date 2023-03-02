#include "Socket.h"

#include <net/if.h>
#include <netlink/cache.h>
#include <netlink/netlink.h>
#include <netlink/route/link/vxlan.h>

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "../VlanMissingException.h"
#include "../logging/easylogging++.h"
#include "Bridge.h"
#include "Link.h"
#include "Vxlan.h"

const std::string VXLAN_IFACE_PREFIX = "vxlan";
const std::string BRIDGE_IFACE_PREFIX = "bridge";

nl::Socket::Socket() {
    socket = nl_socket_alloc();
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

    int err = rtnl_link_vxlan_set_port(vxlan.link, 4789);
    if (err < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_port_range: ") + nl_geterror(err));
    }
    // TODO: properly handle VXLAN iface exists
    err = rtnl_link_add(socket, vxlan.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            LOG(ERROR) << "rtnl_link_add: vxlan interface with vni " << vni << " already exists";
        } else {
            throw std::runtime_error(std::string("error in rtnl_link_add: ") + nl_geterror(err));
        }
    }
}

void nl::Socket::create_bridge_for_vni(uint32_t vni) { create_bridge(BRIDGE_IFACE_PREFIX + std::to_string(vni)); }

void nl::Socket::create_bridge(const std::string &name) {
    Bridge bridge;
    rtnl_link_set_name(bridge.link, name.c_str());
    rtnl_link_set_flags(bridge.link, IFF_UP);

    int err;
    err = rtnl_link_add(socket, bridge.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            LOG(ERROR) << "rtnl_link_add: bridge interface with name " << name << " already exists";
        } else {
            throw std::runtime_error(std::string("error in rtnl_link_add: ") + nl_geterror(err));
        }
    }
}

void nl::Socket::add_iface_bridge(const std::string &bridgeName, const std::string &ifaceName) {
    Bridge bridge;
    Link link;
    int err;

    if ((err = rtnl_link_get_kernel(socket, 0, ifaceName.c_str(), &link.link)) < 0) {
        if (err == -NLE_NODEV) {
            throw VlanMissingException("vlan interface does not exist");
        }
        throw std::runtime_error("Could not get interface: " + ifaceName + " " + nl_geterror(err));
    }
    if ((err = rtnl_link_get_kernel(socket, 0, bridgeName.c_str(), &bridge.link)) < 0) {
        throw std::runtime_error("Could not get interface: " + bridgeName + " " + nl_geterror(err));
    }

    if ((err = rtnl_link_enslave(socket, bridge.link, link.link)) < 0) {
        throw std::runtime_error("Could not enslave interface: " + ifaceName + " to Bridge " + bridgeName + " " +
                                 nl_geterror(err));
    }
}

void nl::Socket::delete_interface(const std::string &name) {
    Link link;
    int err;

    rtnl_link_set_name(link.link, name.c_str());

    if ((err = rtnl_link_delete(socket, link.link)) < 0) {
        throw std::runtime_error(std::string("Could not delete link: ") + nl_geterror(err));
    }
}

std::unordered_set<uint32_t> nl::Socket::interface_list() {
    struct nl_cache *cache;
    int err;
    if ((err = rtnl_link_alloc_cache(socket, AF_INET, &cache)) < 0) {
        throw std::runtime_error(std::string("Could not allocate cache: ") + nl_geterror(err));
    }
    struct nl_object *object;
    object = nl_cache_get_first(cache);
    std::unordered_set<uint32_t> set_of_vnis(0);
    do {
        struct rtnl_link *link;
        link = (rtnl_link *)object;
        if (rtnl_link_is_vxlan(link)) {
            LOG(DEBUG) << rtnl_link_get_name(link);
            uint32_t id;
            if ((err = rtnl_link_vxlan_get_id(link, &id)) < 0) {
                throw std::runtime_error(std::string("Could not get vni ") + nl_geterror(err));
            }
            set_of_vnis.emplace(id);
        }
    } while ((object = nl_cache_get_next(object)) != nullptr);
    return set_of_vnis;
}

static int interface_event_handler(struct nl_msg *msg, void *arg) {
    struct nlmsghdr *nh = nlmsg_hdr(msg);
    if (nh->nlmsg_type != RTM_NEWLINK) {
        return NL_OK;
    }
    auto *ifinfo = static_cast<struct ifinfomsg *>(NLMSG_DATA(nh));
    char ifname[IF_NAMESIZE];
    ifname[0] = '\0';

    if (if_indextoname(ifinfo->ifi_index, &ifname[0]) == nullptr) {
        // TODO: is throwing an exception ok here?
        LOG(ERROR) << "received netlink event for interface with index " << ifinfo->ifi_index
                   << ", but could not get interface name: " << std::strerror(errno);
        return NL_OK;
    }
    auto *sock = static_cast<nl::Socket *>(arg);
    sock->events.push_back(nl::Event{std::string(ifname)});
    return NL_OK;
}

void nl::Socket::subscribe() {
    int err = nl_socket_add_memberships(socket, RTNLGRP_LINK, 0);
    if (err < 0) {
        throw std::runtime_error(std::string("error joining netlink multicast group: ") + nl_geterror(err));
    }

    err = nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, interface_event_handler, this);
    if (err < 0) {
        throw std::runtime_error(std::string("could not set interface callback: ") + nl_geterror(err));
    }

    nl_socket_disable_seq_check(socket);
}

std::vector<nl::Event> nl::Socket::wait_for_events() {
    if (events.empty()) {
        int err = nl_recvmsgs_default(socket);
        if (err < 0) {
            if (err == -NLE_AGAIN) {
                throw TimeoutException("timeout in wait_for_events");
            } else {
                throw std::runtime_error(std::string("could not receive netlink events: ") + nl_geterror(err));
            }
        }
    }
    std::vector events_copy = events;
    events.clear();
    return events_copy;
}

void nl::Socket::set_receive_timeout(const std::chrono::duration<int> &timeout) {
    auto timeout_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    struct timeval tv {};
    tv.tv_sec = timeout_usec.count() / 1000000;
    tv.tv_usec = timeout_usec.count() % 1000000;
    if (setsockopt(nl_socket_get_fd(socket), SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) == -1) {
        throw std::runtime_error(std::string("could not set netlink socket receive timeout: ") + std::strerror(errno));
    }
}
