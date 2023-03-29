#include "Link.h"

nl::Link::Link(struct rtnl_link *link) : link(link) {}

nl::Link::~Link() { rtnl_link_put(link); }
