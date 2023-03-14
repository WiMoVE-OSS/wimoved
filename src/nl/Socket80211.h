#ifndef WM_SOCKET80211_H
#define WM_SOCKET80211_H

#include <netlink/netlink.h>
#include <prometheus/counter.h>

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
    prometheus::Counter& station_counter_received;
   private:
    nl_sock *socket;
};
}

#endif  // WM_SOCKET80211_H
