#include "NetLinkRenderer.h"

void NetLinkRenderer::createVxlanIface(uint32_t vni) {
    std::cout << "Hello" << std::endl;
    struct rtnl_link* link;

    struct nl_addr* addr;
    struct nl_addr* localAddr;

    struct nl_sock* sk;

    int err;

    sk = nl_socket_alloc();
    if (sk == nullptr) {
        throw std::runtime_error(std::string("Could not set id: ") + std::strerror(errno));
    }
    if ((err = nl_connect(sk, NETLINK_ROUTE)) < 0) {
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

    struct ifla_vxlan_port_range range {
        4789, 4789
    };

    if ((err = rtnl_link_vxlan_set_port_range(link, &range)) < 0) {
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

void NetLinkRenderer::deleteInterface(std::string name) {
    struct rtnl_link* link;
    int err;
    struct nl_sock* sk;

    sk = nl_socket_alloc();
    if (sk == nullptr) {
        throw std::runtime_error(std::string("Could not set id: ") + std::strerror(errno));
    }
    if ((err = nl_connect(sk, NETLINK_ROUTE)) < 0) {
        throw std::runtime_error("Tot");
    };

    if (!(link = rtnl_link_alloc())) {
        rtnl_link_put(link);
        throw std::runtime_error("Could not alloccate link");
    }

    rtnl_link_set_name(link, name.c_str());

    if ((err = rtnl_link_delete(sk, link)) < 0) {
        rtnl_link_put(link);
        throw std::runtime_error(std::string("Could not delete link: ") + nl_geterror(err));
    }
    rtnl_link_put(link);
}

void NetLinkRenderer::createBridge(uint32_t id) {
    struct nl_sock* sk;

    int err;

    sk = nl_socket_alloc();
    if (sk == nullptr) {
        throw std::runtime_error(std::string("Could not set id: ") + std::strerror(errno));
    }
    if ((err = nl_connect(sk, NETLINK_ROUTE)) < 0) {
        throw std::runtime_error("Tot");
    };

    struct rtnl_link* link;

    link = rtnl_link_alloc();
    if ((err = rtnl_link_set_type(link, "bridge")) < 0) {
        rtnl_link_put(link);
        std::cout << "Failed set_link_type: " << nl_geterror(err) << std::endl;
        rtnl_link_put(link);
        return;
    }
    rtnl_link_set_name(link, "testbridge");

    if ((err = rtnl_link_add(sk, link, NLM_F_CREATE)) < 0) {
        std::cout << "Failed link_add: " << nl_geterror(err) << std::endl;
        rtnl_link_put(link);
        return;
    }

    struct rtnl_link* slave;
    if ((err = rtnl_link_get_kernel(sk, 0, "vxlan1010", &slave)) < 0) {
        std::cout << "Failed get_kernel lo " << nl_geterror(err) << std::endl;
        rtnl_link_put(link);
        return;
    }
    if ((err = rtnl_link_get_kernel(sk, 0, "testbridge", &link)) < 0) {
        std::cout << "Failed get_kernel bridge" << nl_geterror(err) << std::endl;
        rtnl_link_put(link);
        return;
    }

    if ((err = rtnl_link_enslave(sk, link, slave)) < 0) {
        rtnl_link_put(slave);
        rtnl_link_put(link);
        std::cout << "Failed enslave " << nl_geterror(err) << std::endl;
        return;
    }

    rtnl_link_put(link);
    rtnl_link_put(slave);
}
