#include "Queue.h"

void ipc::Queue::enqueue(std::unique_ptr<ipc::Event> event) {
    {
        std::lock_guard guard(mutex);
        queue.push(std::move(event));
    }
    condition_variable.notify_one();
}

std::unique_ptr<ipc::Event> ipc::Queue::dequeue() {
    std::unique_lock<std::mutex> unique_lock(mutex);
    if (!queue.empty()) {
        return synchronized_dequeue();
    }
    condition_variable.wait(unique_lock);
    return synchronized_dequeue();
}

std::unique_ptr<ipc::Event> ipc::Queue::synchronized_dequeue() {
    std::unique_ptr<Event> event = std::move(queue.front());
    queue.pop();
    return event;
}
