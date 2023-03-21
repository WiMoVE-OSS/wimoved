#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H

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
    void loop_nl_queue(const std::future<void>& future);

    void handle_assoc(ipc::AssocEvent* event) override;
    void handle_auth(ipc::AuthEvent* event) override;
    void handle_disassoc(ipc::DisassocEvent* event) override;
};

#endif  // GAFFA_EVENTLOOP_H
