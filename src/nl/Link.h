#ifndef WIMOVED_LINK_H
#define WIMOVED_LINK_H

#include <netlink/route/link.h>

namespace nl {

class Link {
   public:
    Link();
    Link(const Link &other) = delete;
    ~Link();

    rtnl_link *link;
};

}  // namespace nl

#endif  // WIMOVED_LINK_H
