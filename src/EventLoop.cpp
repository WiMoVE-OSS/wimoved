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

void EventLoop::handle_auth(ipc::AuthEvent* event) {
    WMLOG(DEBUG) << "handle_auth called " << event->station.mac;
    renderer.setup_vni(event->station.vni());
}

void EventLoop::handle_assoc(ipc::AssocEvent* event) {
    event->station.vlan_id = caller.vlan_for_station(event->station.mac);
    WMLOG(DEBUG) << "handle_assoc called " << event->station.mac << " with vlan_id "
                 << event->station.vlan_id.value_or(0);
    WMLOG(INFO) << "Station " << event->station.mac << " connected to AP for VXLAN " << event->station.vni();
    try {
        renderer.setup_station(event->station);
        processing_time_histogram.Observe(event->finished_processing());
    } catch (std::runtime_error& err) {
        WMLOG(ERROR) << "station could not be bridged to vxlan interface: " << err.what()
                     << " - Will now send deauth packet";
        caller.deauth_station(event->station.mac);
    }
}

void EventLoop::handle_disassoc(ipc::DisassocEvent* event) {
    WMLOG(INFO) << "Station " << event->station.mac << " disconnected from AP";
    // WMLOG(DEBUG) << "handle_disassoc called " << event->station_mac ;
}
