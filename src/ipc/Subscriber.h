#ifndef GAFFA_SUBSCRIBER_H
#define GAFFA_SUBSCRIBER_H

#include <future>

#include "../SynchronizedQueue.h"
#include "Socket.h"

namespace ipc {
class Subscriber {
   public:
    Subscriber(SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout,
               const std::string &socket_path);

    void loop(const std::future<void> &future);

   private:
    Socket socket;
    SynchronizedQueue<Event> &queue;
};
}  // namespace ipc

#endif  // GAFFA_SUBSCRIBER_H
