#include "Queue.h"
#include "TimeoutException.h"

void ipc::Queue::enqueue(std::unique_ptr<ipc::Event> event) {
    {
        std::lock_guard guard(mutex);
        queue.push(std::move(event));
    }
    condition_variable.notify_one();
}

std::unique_ptr<ipc::Event> ipc::Queue::dequeue(const std::chrono::duration<int>& timeout) {
    std::unique_lock<std::mutex> unique_lock(mutex);
    if (!queue.empty()) {
        return synchronized_dequeue();
    }
    if (condition_variable.wait_for(unique_lock, timeout) == std::cv_status::timeout) {
        throw TimeoutException("timeout in dequeue()");
    }
    return synchronized_dequeue();
}

std::unique_ptr<ipc::Event> ipc::Queue::synchronized_dequeue() {
    std::unique_ptr<Event> event = std::move(queue.front());
    queue.pop();
    return event;
}
