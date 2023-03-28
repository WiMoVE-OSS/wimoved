#include "LinkCache.h"

#include <netlink/route/link.h>

#include <stdexcept>

LinkCache::LinkCache(struct nl_sock *socket) {
    int err = rtnl_link_alloc_cache(socket, AF_INET, &cache);
    if (err < 0) {
        throw std::runtime_error(std::string("Could not allocate cache: ") + nl_geterror(err));
    }
}
LinkCache::~LinkCache() { nl_cache_put(cache); }
