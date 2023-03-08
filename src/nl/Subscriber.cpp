#include "Subscriber.h"

#include <iostream>

#include "../logging/loginit.h"
#include "../metrics/MetricsManager.h"
#include "Event.h"
#include "prometheus/counter.h"

nl::Subscriber::Subscriber(::SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout)
    : queue(queue), socket() {
    socket.subscribe();
    socket.set_receive_timeout(timeout);
}

void nl::Subscriber::loop(const std::future<void> &future) {
    prometheus::Counter &receivedNetlinkCounter = MetricsManager::get_instance().get_netlink_counter_received();
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::vector<Event> events;
        try {
            events = socket.wait_for_events();
        } catch (TimeoutException &) {
            continue;
        }
        for (auto &event : events) {
            receivedNetlinkCounter.Increment();
            queue.enqueue(std::make_unique<Event>(std::move(event)));
        }
    }
}
