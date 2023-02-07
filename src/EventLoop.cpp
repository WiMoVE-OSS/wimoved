//
// Created by richard on 07.02.23.
//

#include <iostream>
#include "EventLoop.h"

EventLoop::EventLoop(IPCQueue &queue) : queue(queue) {
}

void EventLoop::loop() {
    while (true) {
        std::unique_ptr<IPCEvent> event = queue.dequeue();
        event->handle(this);
    }
}

void EventLoop::handle_assoc(const IPCAssocEvent *event) {
    std::cout << "handle_assoc called" << std::endl;
}

void EventLoop::handle_auth(const IPCAuthEvent *event) {
    std::cout << "handle_auth called" << std::endl;
}

void EventLoop::handle_disassoc(const IPCDisassocEvent *event) {
    std::cout << "handle_disassoc called" << std::endl;
}

