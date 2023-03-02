#include <iostream>
#include <thread>
#include "EventLoop.h"
#include "NetworkRenderer.h"
#include "TimeoutException.h"
#include "nl/Event.h"
#include "VlanMissingException.h"

EventLoop::EventLoop(NetworkRenderer& renderer, SynchronizedQueue<ipc::Event> &ipc_queue, SynchronizedQueue<nl::Event> &nl_queue, const std::string& socket_path) : renderer(renderer), ipc_queue(ipc_queue), nl_queue(nl_queue), caller(socket_path), stations_without_interface(), loop_mutex() {
}

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
            std::cerr << "received netlink event for iface " << event->interface_name << std::endl;
            auto event_iterator = stations_without_interface.find(event->interface_name);
            std::lock_guard g(loop_mutex);
            if (event_iterator != stations_without_interface.end()) {
                std::cerr << "setting up interface " << event->interface_name << " after netlink event" << std::endl;
                try {
                    renderer.setup_station(event_iterator->second);
                } catch (const VlanMissingException& e) {
                    std::cerr << "error setting up interface " << event->interface_name << ": " << e.what() << std::endl;
                } catch (const std::runtime_error &e) {
                    std::cerr << "station could not be bridged to vxlan interface: " << e.what() << std::endl;
                }
                stations_without_interface.erase(event_iterator);
            }
        } catch (const TimeoutException& e) {
            continue;
        }
    }
}

void EventLoop::handle_auth(ipc::AuthEvent *event) {
    std::cout << "handle_auth called " << event->station.mac << std::endl;
    renderer.setup_vni(event->station.vni());
}

void EventLoop::handle_assoc(ipc::AssocEvent *event) {
    event->station.vlan_id = caller.vlan_for_station(event->station.mac);
    std::cerr << "handle_assoc called " << event->station.mac << " with vlan_id " << event->station.vlan_id.value_or(0) << std::endl;
    try {
        renderer.setup_station(event->station);
    }
    catch (VlanMissingException&) {
        std::cerr << "vlan interface " << event->station.vlan_interface_name() << " missing in setup_station" << std::endl;
        stations_without_interface.emplace(event->station.vlan_interface_name(), event->station);
    }
    catch (std::runtime_error &err) {
        std::cerr << "station could not be bridged to vxlan interface: " << err.what() << std::endl;
    }
    //TODO: Disconnect station on bridging failure
}

void EventLoop::handle_disassoc(ipc::DisassocEvent *event) {
    //std::cout << "handle_disassoc called " << event->station_mac << std::endl;
}

