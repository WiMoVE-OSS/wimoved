#ifndef WIMOVED_EVENTLOOP_H
#define WIMOVED_EVENTLOOP_H

#include <future>

#include "EventHandler.h"
#include "NetworkRenderer.h"
#include "SynchronizedQueue.h"
#include "ipc/Caller.h"
#include "nl/Socket.h"
#include "prometheus/histogram.h"

class EventLoop : public EventHandler {
   private:
    NetworkRenderer& renderer;
    SynchronizedQueue<ipc::Event>& ipc_queue;
    ipc::Caller caller;
    prometheus::Histogram& processing_time_histogram;

   public:
    EventLoop(NetworkRenderer& renderer, SynchronizedQueue<ipc::Event>& ipc_queue);

    void loop_ipc_queue(const std::future<void>& future);

    void handle_connect(ipc::ConnectEvent* event) override;
    void handle_disconnect(ipc::DisconnectEvent* event) override;
};

#endif  // WIMOVED_EVENTLOOP_H
