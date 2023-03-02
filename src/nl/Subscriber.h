#ifndef GAFFA_NL_SUBSCRIBER_H
#define GAFFA_NL_SUBSCRIBER_H

#include <future>

#include "../SynchronizedQueue.h"
#include "Event.h"
#include "Socket.h"

namespace nl {
class Subscriber {
   public:
    Subscriber(SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout);

    void loop(const std::future<void> &future);

   private:
    ::SynchronizedQueue<Event> &queue;
    Socket socket;
};
}  // namespace nl

#endif  // GAFFA_NL_SUBSCRIBER_H
