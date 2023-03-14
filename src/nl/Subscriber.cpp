#include "Subscriber.h"

#include "../logging/loginit.h"
#include "../metrics/MetricsManager.h"
#include "prometheus/counter.h"

nl::Subscriber::Subscriber(::SynchronizedQueue<Station> &queue, const std::chrono::duration<int> &timeout)
    : queue(queue), socket(timeout) {
}

void nl::Subscriber::loop(const std::future<void> &future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::vector<Station> stations;
        try {
            stations = socket.wait_for_stations();
        } catch (TimeoutException &) {
            continue;
        }
        for (auto &station : stations) {
            queue.enqueue(std::make_unique<Station>(std::move(station)));
        }
    }
}
