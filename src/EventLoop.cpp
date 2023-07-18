#include "EventLoop.h"

#include <iostream>
#include <thread>

#include "NetworkRenderer.h"
#include "TimeoutException.h"
#include "logging/loginit.h"
#include "metrics/MetricsManager.h"

EventLoop::EventLoop(NetworkRenderer& renderer, SynchronizedQueue<ipc::Event>& ipc_queue)
    : renderer(renderer),
      ipc_queue(ipc_queue),
      caller(),
      processing_time_histogram(MetricsManager::get_instance().get_event_histogram()) {}

void EventLoop::loop_ipc_queue(const std::future<void>& future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::unique_ptr<ipc::Event> event;
        try {
            event = ipc_queue.dequeue(std::chrono::seconds(1));
        } catch (const TimeoutException& e) {
            continue;
        }
        event->handle(this);
    }
}

void EventLoop::handle_connect(ipc::ConnectEvent* event) {
    event->station.vlan_id = caller.vlan_for_station(event->station);
    WMLOG(DEBUG) << "Entering handle_connect station=" << event->station
                 << " vlan_id=" << event->station.vlan_id.value_or(0);
    WMLOG(INFO) << "Connected iface=" << event->station.sockname << " station=" << event->station.mac.string()
                << " vni=" << event->station.vni();
    try {
        renderer.setup_station(event->station);
        processing_time_histogram.Observe(static_cast<double>(event->finished_processing()));
    } catch (std::runtime_error& err) {
        WMLOG(ERROR) << "Could not connect - Will now send DEAUTH packet. error=" << err.what()
                     << " iface=" << event->station.sockname << " station=" << event->station.mac.string()
                     << " vni=" << event->station.vni();
        caller.deauth_station(event->station);
    }
}

void EventLoop::handle_disconnect(ipc::DisconnectEvent* event) {
    WMLOG(INFO) << "Disconnected station=" << event->station.mac.string();
}
