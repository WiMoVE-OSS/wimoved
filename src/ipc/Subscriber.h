#ifndef GAFFA_SUBSCRIBER_H
#define GAFFA_SUBSCRIBER_H

#include <future>
#include "Socket.h"
#include "Queue.h"

namespace ipc {
    class Subscriber {
    public:
        explicit Subscriber(Queue &queue, const std::string &socket_path);

        void loop(const std::future<void>& future);

    private:
        Socket socket;
        Queue &queue;
    };
}

#endif // GAFFA_SUBSCRIBER_H
