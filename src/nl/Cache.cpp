#include "Cache.h"

#include <netlink/cache.h>

Cache::Cache(nl_cache *cache) : cache(cache) {}

Cache::~Cache() { nl_cache_put(cache); }
