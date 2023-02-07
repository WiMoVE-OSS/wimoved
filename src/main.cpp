#include <iostream>
#include <thread>
#include "IPCSubscriber.h"

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
    while (true) {
        IPCEvent event = queue.dequeue();
        std::cout << "event from queue: " << event.content() << std::endl;
    }
    subscriber_thread.join();
}