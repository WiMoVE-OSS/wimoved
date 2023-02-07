//
// Created by richard on 07.02.23.
//

#include <iostream>
#include "EventLoop.h"

EventLoop::EventLoop(IPCQueue &queue) : queue(queue) {
}

void EventLoop::loop() {
    while (true) {
        IPCEvent event = queue.dequeue();
        event.handle(this);
    }
}

void EventLoop::handle_assoc(const IPCEvent *event) {
    std::cout << "handle_assoc on " << event->content() << std::endl;
}

void EventLoop::handle_auth(const IPCEvent *event) {
    std::cout << "handle_auth on " << event->content() << std::endl;
}

void EventLoop::handle_disassoc(const IPCEvent *event) {
    std::cout << "handle_disassoc on " << event->content() << std::endl;
}

