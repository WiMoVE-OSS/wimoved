#ifndef WIMOVED_CACHE_H
#define WIMOVED_CACHE_H

class Cache {
   public:
    explicit Cache(struct nl_cache* cache);
    ~Cache();
    Cache(const Cache& other) = delete;
    Cache operator=(const Cache& other) = delete;

    struct nl_cache* cache = nullptr;
};

#endif  // WIMOVED_CACHE_H
