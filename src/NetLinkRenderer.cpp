#include "NetLinkRenderer.h"

void NetLinkRenderer::createVxlanIface(uint32_t vni) {
    std::cout << "Hello" << std::endl;
    struct rtnl_link *link;

    struct nl_addr* addr;
    struct nl_addr* localAddr;

    struct nl_sock* sk;

    int err;

    sk = nl_socket_alloc();
    if (sk == nullptr) {
        throw std::runtime_error(std::string("Could not set id: ") + std::strerror(errno));
    }
    if ((err = nl_connect(sk, NETLINK_ROUTE )) < 0){
        throw std::runtime_error("Tot");
    };

    /* allocate new link object of type vxlan */
    link = rtnl_link_vxlan_alloc();

    if (link == nullptr) {
        throw std::runtime_error("Not allocated");
    }

    /* set interface name */
    rtnl_link_set_name(link, ("vxlan" + std::to_string(vni)).c_str());

    /* set VXLAN network identifier */
    if ((err = rtnl_link_vxlan_set_id(link, vni)) < 0) {
        throw std::runtime_error(std::string("Could not set id: ") + std::strerror(errno));
    }

    struct ifla_vxlan_port_range range{4789, 4789};

    if ((err = rtnl_link_vxlan_set_port_range(link, &range))< 0) {
        throw std::runtime_error(std::string("Could not set portrange") + nl_geterror(err));
    }


    if ((err = rtnl_link_add(sk, link, NLM_F_CREATE)) < 0) {
        if (err == NLE_EXIST) {
            std::cerr << "Interface with this vni already exists: " << vni << std::endl;
        } else {
            std::cout << nl_geterror(err) << std::endl;
            throw std::runtime_error("TEST");
        }
    }
    rtnl_link_put(link);
}
