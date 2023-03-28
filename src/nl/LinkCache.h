#ifndef WIMOVED_LINKCACHE_H
#define WIMOVED_LINKCACHE_H

class LinkCache {
   public:
    LinkCache(struct nl_sock* socket);
    ~LinkCache();
    LinkCache(const LinkCache& other) = delete;
    LinkCache operator=(const LinkCache& other) = delete;

    struct nl_cache* cache;
};

#endif  // WIMOVED_LINKCACHE_H
