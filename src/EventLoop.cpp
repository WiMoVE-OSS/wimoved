#include <iostream>
#include "EventLoop.h"
#include "NetworkRenderer.h"

EventLoop::EventLoop(NetworkRenderer& renderer, IPCQueue &queue, const std::string& iface) : renderer(renderer), queue(queue), caller(iface) {
}

void EventLoop::loop() {
    while (true) {
        std::unique_ptr<IPCEvent> event = queue.dequeue();
        event->handle(this);
    }
}

void EventLoop::handle_auth(IPCAuthEvent *event) {
    //std::cout << "handle_auth called " << event->station_mac << std::endl;
    renderer.setup_vni(event->station.vni());
}

void EventLoop::handle_assoc(IPCAssocEvent *event) {
    event->station.vlan_id = caller.vlan_for_station(event->station.mac);
    //std::cout << "handle_assoc called " << event->station_mac << " with vlan_id " << event->vlan_id << std::endl;
    renderer.setup_station(event->station);
}

void EventLoop::handle_disassoc(IPCDisassocEvent *event) {
    //std::cout << "handle_disassoc called " << event->station_mac << std::endl;
}

