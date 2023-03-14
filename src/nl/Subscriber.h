#ifndef WM_NL_SUBSCRIBER_H
#define WM_NL_SUBSCRIBER_H

#include <future>

#include "../SynchronizedQueue.h"
#include "Socket80211.h"

namespace nl {
class Subscriber {
   public:
    Subscriber(SynchronizedQueue<Station> &queue, const std::chrono::duration<int> &timeout);

    void loop(const std::future<void> &future);

   private:
    ::SynchronizedQueue<Station> &queue;
    Socket80211 socket;
};
}  // namespace nl

#endif  // WM_NL_SUBSCRIBER_H
