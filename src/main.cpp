#include <iostream>
#include <thread>
#include "BridgePerVxlanRenderer.h"
#include "ipc/Subscriber.h"
#include "EventLoop.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    std::string iface = argv[1];
    ipc::Queue queue;
    BridgePerVxlanRenderer renderer;

    std::thread subscriber_thread([&queue, &iface](){
        ipc::Subscriber(queue, iface).loop();
    });
    std::thread event_loop_thread([&renderer, &queue, &iface]() {
        EventLoop(renderer, queue, iface).loop();
    });
    std::thread cleanup_thread([&renderer, &iface]() {
        ipc::Caller caller(iface);
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            renderer.cleanup([&caller]() {
                return caller.connected_stations();
            });
        }
    });
    cleanup_thread.join();
    subscriber_thread.join();
    event_loop_thread.join();
}