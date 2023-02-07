#ifndef GAFFA_IPCQUEUE_H
#define GAFFA_IPCQUEUE_H


#include <cstdlib>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "IPCEvent.h"

class IPCQueue {

private:
    std::queue<IPCEvent> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;
    IPCEvent synchronized_dequeue();
public:
    void enqueue(const IPCEvent& event);
    IPCEvent dequeue();
};


#endif //GAFFA_IPCQUEUE_H
