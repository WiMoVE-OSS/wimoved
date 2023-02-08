#ifndef GAFFA_NETLINKVXLAN_H
#define GAFFA_NETLINKVXLAN_H

#include <netlink/route/link/vxlan.h>

class NetlinkVxlan {
public:
    NetlinkVxlan();
    NetlinkVxlan(const NetlinkVxlan& other) = delete;
    ~NetlinkVxlan();
    rtnl_link *link;
};


#endif //GAFFA_NETLINKVXLAN_H
