#ifndef WIMOVED_LINK_H
#define WIMOVED_LINK_H

#include <netlink/route/link.h>

namespace nl {

class Link {
   public:
    explicit Link(rtnl_link *link);
    ~Link();
    Link(const Link& other) = delete;
    Link operator=(const Link& other) = delete;
    Link(Link&& other) = delete;
    Link operator=(Link&& other) = delete;

    rtnl_link* link;
};

}  // namespace nl

#endif  // WIMOVED_LINK_H
