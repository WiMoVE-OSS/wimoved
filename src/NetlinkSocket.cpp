#include <stdexcept>
#include <cstring>
#include <netlink/netlink.h>
#include <netlink/route/link/vxlan.h>
#include <net/if.h>
#include <iostream>
#include "NetlinkSocket.h"
#include "NetlinkVxlan.h"

const std::string VXLAN_IFACE_PREFIX = "vxlan";

NetlinkSocket::NetlinkSocket() {
    socket = nl_socket_alloc();
    if (socket == nullptr) {
        throw std::runtime_error(std::string("error in nl_socket_alloc: ") + std::strerror(errno));
    }
    int err = nl_connect(socket, NETLINK_ROUTE);
    if (err < 0) {
        throw std::runtime_error(std::string("error in nl_connect: ") + nl_geterror(err));
    };
}

NetlinkSocket::~NetlinkSocket() {
    nl_close(socket);
    nl_socket_free(socket);
}

void NetlinkSocket::create_vxlan_iface(uint32_t vni) {
    NetlinkVxlan vxlan;

    rtnl_link_set_name(vxlan.link, (VXLAN_IFACE_PREFIX + std::to_string(vni)).c_str());
    rtnl_link_set_flags(vxlan.link, IFF_UP);

    if (rtnl_link_vxlan_set_id(vxlan.link, vni) < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_id: ") + std::strerror(errno));
    }

    struct ifla_vxlan_port_range range{4789, 4789};
    int err = rtnl_link_vxlan_set_port_range(vxlan.link, &range);
    if (err < 0) {
        throw std::runtime_error(std::string("error in rtnl_link_vxlan_set_port_range: ") + nl_geterror(err));
    }
    // TODO: properly handle VXLAN iface exists
    err = rtnl_link_add(socket, vxlan.link, NLM_F_CREATE | NLM_F_EXCL);
    if (err < 0) {
        if (err == -NLE_EXIST) {
            std::cout << "rtnl_link_add: vxlan interface with vni " << vni << " already exists" << std::endl;
        } else {
            throw std::runtime_error(std::string("error in rtnl_link_add: ") + nl_geterror(err));
        }
    }
}

