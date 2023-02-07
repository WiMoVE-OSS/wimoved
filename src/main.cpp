#include <iostream>
#include <thread>
#include "IPCSubscriber.h"
#include "EventLoop.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    std::string iface = argv[1];
    IPCQueue queue;

    std::thread subscriber_thread([&queue, &iface](){
        IPCSubscriber(queue, iface).loop();
    });
    std::thread event_loop_thread([&queue, &iface]() {
        EventLoop(queue, iface).loop();
    });
    subscriber_thread.join();
    event_loop_thread.join();
}