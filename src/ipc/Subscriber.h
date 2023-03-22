#ifndef WIMOVED_SUBSCRIBER_H
#define WIMOVED_SUBSCRIBER_H

#include <future>

#include "../SynchronizedQueue.h"
#include "Socket.h"
#include "prometheus/counter.h"

namespace ipc {
class Subscriber {
   public:
    Subscriber(SynchronizedQueue<Event> &queue, const std::chrono::duration<int> &timeout);

    void loop(const std::future<void> &future);

   private:
    Socket socket;
    SynchronizedQueue<Event> &queue;
    prometheus::Counter &hostapd_disconnect_counter;
    prometheus::Counter &hostapd_connect_counter;
    prometheus::Counter &hostapd_unknown_counter;
    std::pair<std::string, std::string> split_at_first_space(const std::string &line);
    std::string get_ifname(const std::string &eventprefix);
};
}  // namespace ipc

#endif  // WIMOVED_SUBSCRIBER_H
