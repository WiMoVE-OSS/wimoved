#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H

#include <future>

#include "EventHandler.h"
#include "NetworkRenderer.h"
#include "SynchronizedQueue.h"
#include "ipc/Caller.h"
#include "nl/Socket.h"
#include "nl/Socket80211.h"

class EventLoop {
   private:
    NetworkRenderer& renderer;
    SynchronizedQueue<Station>& station_queue;
    nl::Socket80211 socket;

   public:
    EventLoop(NetworkRenderer& renderer, SynchronizedQueue<Station>& nl_queue);
    void loop_nl_queue(const std::future<void>& future);
};

#endif  // GAFFA_EVENTLOOP_H
