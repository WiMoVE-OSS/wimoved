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
    WMLOG(DEBUG) << "handle_connect called " << event->station.mac << " with vlan_id "
                 << event->station.vlan_id.value_or(0);
    WMLOG(INFO) << "Station " << event->station.mac << " connected to AP for VXLAN " << event->station.vni()
                << " at interface " << event->station.ifname;
    try {
        renderer.setup_station(event->station);
        processing_time_histogram.Observe(event->finished_processing());
    } catch (std::runtime_error& err) {
        WMLOG(ERROR) << "station could not be bridged to vxlan interface: " << err.what()
                     << " - Will now send deauth packet";
        caller.deauth_station(event->station);
    }
}

void EventLoop::handle_disconnect(ipc::DisconnectEvent* event) {
    WMLOG(INFO) << "Station " << event->station.mac << " disconnected from AP";
}
