#include "IPCQueue.h"

void IPCQueue::enqueue(const IPCEvent& event) {
    {
        std::lock_guard guard(mutex);
        queue.push(event);
    }
    condition_variable.notify_one();
}

IPCEvent IPCQueue::dequeue() {
    std::unique_lock<std::mutex> unique_lock(mutex);
    if (!queue.empty()) {
        return synchronized_dequeue();
    }
    condition_variable.wait(unique_lock);
    return synchronized_dequeue();
}

IPCEvent IPCQueue::synchronized_dequeue() {
    IPCEvent event = queue.front();
    queue.pop();
    return event;
}
