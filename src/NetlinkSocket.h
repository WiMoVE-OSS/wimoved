#ifndef GAFFA_NETLINKSOCKET_H
#define GAFFA_NETLINKSOCKET_H


class NetlinkSocket {
public:
    NetlinkSocket();
    ~NetlinkSocket();
    void create_vxlan_iface(uint32_t vni);
private:
    struct nl_sock *socket;
};


#endif //GAFFA_NETLINKSOCKET_H
