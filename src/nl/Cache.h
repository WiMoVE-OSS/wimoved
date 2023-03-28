#ifndef WIMOVED_CACHE_H
#define WIMOVED_CACHE_H

#include <netlink/cache.h>

class Cache {
   public:
    explicit Cache(struct nl_cache* cache);
    ~Cache();
    Cache(const Cache& other) = delete;
    Cache operator=(const Cache& other) = delete;
    Cache(Cache&& other) = delete;
    Cache operator=(Cache&& other) = delete;

    nl_cache* cache;
};

#endif  // WIMOVED_CACHE_H
