#ifndef GAFFA_SUBSCRIBER_H
#define GAFFA_SUBSCRIBER_H

#include "Socket.h"
#include "Queue.h"

namespace ipc {
    class Subscriber {
    public:
        explicit Subscriber(Queue &queue, const std::string &iface);

        void loop();

    private:
        Socket socket;
        Queue &queue;
    };
}

#endif // GAFFA_SUBSCRIBER_H
