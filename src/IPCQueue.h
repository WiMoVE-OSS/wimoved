#ifndef GAFFA_IPCQUEUE_H
#define GAFFA_IPCQUEUE_H


#include <cstdlib>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "IPCEvent.h"

class IPCQueue {

private:
    std::queue<std::unique_ptr<IPCEvent>> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;
    std::unique_ptr<IPCEvent> synchronized_dequeue();
public:
    void enqueue(std::unique_ptr<IPCEvent> event);
    std::unique_ptr<IPCEvent> dequeue();
};


#endif //GAFFA_IPCQUEUE_H
