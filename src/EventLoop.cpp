#include "EventLoop.h"

#include <iostream>
#include <thread>

#include "NetworkRenderer.h"
#include "TimeoutException.h"
#include "VlanMissingException.h"
#include "logging/loginit.h"
#include "nl/Event.h"

EventLoop::EventLoop(NetworkRenderer& renderer, SynchronizedQueue<ipc::Event>& ipc_queue,
                     SynchronizedQueue<nl::Event>& nl_queue)
    : renderer(renderer),
      ipc_queue(ipc_queue),
      nl_queue(nl_queue),
      caller(),
      stations_without_interface(),
      loop_mutex() {}

void EventLoop::loop_ipc_queue(const std::future<void>& future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::unique_ptr<ipc::Event> event;
        try {
            event = ipc_queue.dequeue(std::chrono::seconds(1));
        } catch (const TimeoutException& e) {
            continue;
        }
        std::lock_guard g(loop_mutex);
        event->handle(this);
    }
}

void EventLoop::loop_nl_queue(const std::future<void>& future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::unique_ptr<nl::Event> event;
        try {
            event = nl_queue.dequeue(std::chrono::seconds(1));
            GAFFALOG(DEBUG) << "received netlink event for iface " << event->interface_name;
            auto event_iterator = stations_without_interface.find(event->interface_name);
            std::lock_guard g(loop_mutex);
            if (event_iterator != stations_without_interface.end()) {
                GAFFALOG(DEBUG) << "setting up interface " << event->interface_name << " after netlink event";
                try {
                    renderer.setup_station(event_iterator->second);
                } catch (const VlanMissingException& e) {
                    GAFFALOG(ERROR) << "error setting up interface " << event->interface_name << ": " << e.what();
                } catch (const std::runtime_error& e) {
                    GAFFALOG(ERROR) << "station could not be bridged to vxlan interface: " << e.what();
                }
                stations_without_interface.erase(event_iterator);
            }
        } catch (const TimeoutException& e) {
            continue;
        }
    }
}

void EventLoop::handle_auth(ipc::AuthEvent* event) {
    GAFFALOG(DEBUG) << "handle_auth called " << event->station.mac;
    renderer.setup_vni(event->station.vni());
}

void EventLoop::handle_assoc(ipc::AssocEvent* event) {
    event->station.vlan_id = caller.vlan_for_station(event->station.mac);
    GAFFALOG(DEBUG) << "handle_assoc called " << event->station.mac << " with vlan_id "
                    << event->station.vlan_id.value_or(0);
    GAFFALOG(INFO) << "Station " << event->station.mac << " connected to AP for VXLAN " << event->station.vni();
    try {
        renderer.setup_station(event->station);
    } catch (VlanMissingException&) {
        GAFFALOG(WARNING) << "vlan interface " << event->station.vlan_interface_name()
                          << " missing in setup_station. Waiting for it to be created.";
        stations_without_interface.emplace(event->station.vlan_interface_name(), event->station);
    } catch (std::runtime_error& err) {
        GAFFALOG(ERROR) << "station could not be bridged to vxlan interface: " << err.what();
    }
    // TODO: Disconnect station on bridging failure
}

void EventLoop::handle_disassoc(ipc::DisassocEvent* event) {
    GAFFALOG(INFO) << "Station " << event->station.mac << " disconnected from AP";
    // GAFFALOG(DEBUG) << "handle_disassoc called " << event->station_mac ;
}
