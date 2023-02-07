#include <iostream>
#include "EventLoop.h"

EventLoop::EventLoop(IPCQueue &queue, const std::string& iface) : queue(queue), caller(iface) {
}

void EventLoop::loop() {
    while (true) {
        std::unique_ptr<IPCEvent> event = queue.dequeue();
        event->handle(this);
    }
}

void EventLoop::handle_assoc(IPCAssocEvent *event) {
    event->vlan_id = caller.vlan_for_station(event->station_mac);
    std::cout << "handle_assoc called " << event->station_mac << " with vlan_id " << event->vlan_id << std::endl;
}

void EventLoop::handle_auth(IPCAuthEvent *event) {
    std::cout << "handle_auth called " << event->station_mac << std::endl;
}

void EventLoop::handle_disassoc(IPCDisassocEvent *event) {
    std::cout << "handle_disassoc called " << event->station_mac << std::endl;
}

