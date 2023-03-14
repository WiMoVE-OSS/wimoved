#ifndef WM_EVENTLOOP_H
#define WM_EVENTLOOP_H

#include <future>

#include "NetworkRenderer.h"
#include "SynchronizedQueue.h"
#include "ipc/Caller.h"
#include "nl/Socket.h"
#include "nl/Socket80211.h"
#include "prometheus/histogram.h"

class EventLoop {
   private:
    NetworkRenderer& renderer;
    SynchronizedQueue<Station>& station_queue;
    nl::Socket80211 socket;
    prometheus::Histogram& processing_time_histogram;

   public:
    EventLoop(NetworkRenderer& renderer, SynchronizedQueue<Station>& nl_queue);
    void loop_nl_queue(const std::future<void>& future);
};

#endif  // WM_EVENTLOOP_H
