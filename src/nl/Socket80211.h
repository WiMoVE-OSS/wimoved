#ifndef GAFFA_SOCKET80211_H
#define GAFFA_SOCKET80211_H

#include <netlink/netlink.h>

#include <chrono>
#include <unordered_set>
#include <vector>

#include "../Station.h"

namespace nl {
class Socket80211 {
   public:
    Socket80211(const std::chrono::duration<int>& timeout);
    ~Socket80211();
    std::vector<Station> wait_for_stations();

    std::vector<Station> new_stations;
   private:
    nl_sock *socket;
};
}

#endif  // GAFFA_SOCKET80211_H
