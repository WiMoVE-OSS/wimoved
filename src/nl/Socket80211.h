#ifndef GAFFA_SOCKET80211_H
#define GAFFA_SOCKET80211_H

#include <netlink/netlink.h>

#include <chrono>
#include <unordered_set>
#include <vector>

#include "../Station.h"
#include "Event.h"

namespace nl {
class Socket80211 {
   public:
    Socket80211();
    ~Socket80211();
    std::vector<Station> wait_for_stations();
    //Station station_for_event(const Event& event);
    void set_receive_timeout(const std::chrono::duration<int>& timeout);
    void subscribe();

    std::vector<Station> new_stations;
   private:
    nl_sock *socket;
};
}

#endif  // GAFFA_SOCKET80211_H
