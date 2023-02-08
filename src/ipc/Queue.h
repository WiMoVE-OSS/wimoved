#ifndef GAFFA_QUEUE_H
#define GAFFA_QUEUE_H


#include <cstdlib>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Event.h"

namespace ipc {
    class Queue {

    private:
        std::queue<std::unique_ptr<Event>> queue;
        std::mutex mutex;
        std::condition_variable condition_variable;

        std::unique_ptr<Event> synchronized_dequeue();

    public:
        void enqueue(std::unique_ptr<Event> event);

        std::unique_ptr<Event> dequeue();
    };
}


#endif //GAFFA_QUEUE_H
