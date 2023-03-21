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
    prometheus::Counter &hostapd_association_counter;
    prometheus::Counter &hostapd_disassociation_counter;
    prometheus::Counter &hostapd_authentication_counter;
    prometheus::Counter &hostapd_unknown_counter;
};
}  // namespace ipc

#endif  // WIMOVED_SUBSCRIBER_H
