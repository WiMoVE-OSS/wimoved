#include "Subscriber.h"

#include <iostream>

#include "../logging/easylogging++.h"
#include "Event.h"

nl::Subscriber::Subscriber(::SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout)
    : queue(queue), socket() {
    socket.subscribe();
    socket.set_receive_timeout(timeout);
}

void nl::Subscriber::loop(const std::future<void> &future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::vector<Event> events;
        try {
            events = socket.wait_for_events();
        } catch (TimeoutException &) {
            continue;
        }
        for (auto &event : events) {
            LOG(ERROR) << event.interface_name;
            queue.enqueue(std::make_unique<Event>(std::move(event)));
        }
    }
}
