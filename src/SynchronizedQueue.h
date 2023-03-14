#ifndef GAFFA_SYNCHRONIZEDQUEUE_H
#define GAFFA_SYNCHRONIZEDQUEUE_H

#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <queue>

#include "TimeoutException.h"

template <typename T>
class SynchronizedQueue {
   private:
    std::queue<std::unique_ptr<T>> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;

    std::unique_ptr<T> synchronized_dequeue() {
        std::unique_ptr<T> event = std::move(queue.front());
        queue.pop();
        return event;
    }

   public:
    void enqueue(std::unique_ptr<T> event) {
        {
            std::lock_guard guard(mutex);
            queue.push(std::move(event));
        }
        condition_variable.notify_one();
    }

    std::unique_ptr<T> dequeue(const std::chrono::duration<int>& timeout) {
        std::unique_lock<std::mutex> unique_lock(mutex);
        if (!queue.empty()) {
            return synchronized_dequeue();
        }
        if (condition_variable.wait_for(unique_lock, timeout) == std::cv_status::timeout) {
            throw TimeoutException("timeout in dequeue()");
        }
        return synchronized_dequeue();
    }
};

#endif  // GAFFA_SYNCHRONIZEDQUEUE_H
