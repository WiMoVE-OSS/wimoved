#ifndef GAFFA_NL_SUBSCRIBER_H
#define GAFFA_NL_SUBSCRIBER_H


#include <future>
#include "Event.h"
#include "../SynchronizedQueue.h"
#include "Socket.h"

namespace nl {
    class Subscriber {
    public:
        Subscriber(SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout);

        void loop(const std::future<void>& future);
    private:
        ::SynchronizedQueue<Event> &queue;
        Socket socket;
    };
}

#endif //GAFFA_NL_SUBSCRIBER_H
