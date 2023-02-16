#include <iostream>
#include <thread>
#include "EventLoop.h"
#include "NetworkRenderer.h"
#include "ipc/TimeoutException.h"

EventLoop::EventLoop(NetworkRenderer& renderer, ipc::Queue &queue, const std::string& iface) : renderer(renderer), queue(queue), caller(iface) {
}

void EventLoop::loop(const std::future<void>& future) {
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::unique_ptr<ipc::Event> event;
        try {
            event = queue.dequeue(std::chrono::seconds(1));
        } catch (const ipc::TimeoutException& e) {
            continue;
        }
        event->handle(this);
    }
}

void EventLoop::handle_auth(ipc::AuthEvent *event) {
    //std::cout << "handle_auth called " << event->station_mac << std::endl;
    renderer.setup_vni(event->station.vni());
}

void EventLoop::handle_assoc(ipc::AssocEvent *event) {
    event->station.vlan_id = caller.vlan_for_station(event->station.mac);
    //std::cout << "handle_assoc called " << event->station_mac << " with vlan_id " << event->vlan_id << std::endl;
    for (int i = 0; i < 10; i++) {
        try {
            renderer.setup_station(event->station);
            return;
        }
        catch (std::runtime_error &err) {
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    //TODO: Disconnect station on bridging failure
    std::cerr << "Station could not be bridged to vxlan interface " << std::endl;
}

void EventLoop::handle_disassoc(ipc::DisassocEvent *event) {
    //std::cout << "handle_disassoc called " << event->station_mac << std::endl;
}

