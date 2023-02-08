#include <iostream>
#include <thread>
#include "ipc/IPCSubscriber.h"
#include "EventLoop.h"
#include "BridgePerVxlanRenderer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    std::string iface = argv[1];
    IPCQueue queue;
    BridgePerVxlanRenderer renderer;

    std::thread subscriber_thread([&queue, &iface](){
        IPCSubscriber(queue, iface).loop();
    });
    std::thread event_loop_thread([&renderer, &queue, &iface]() {
        EventLoop(renderer, queue, iface).loop();
    });
    std::thread cleanup_thread([&renderer, &iface]() {
        IPCCaller caller(iface);
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            std::vector<std::string> stations = caller.connected_stations();
            renderer.cleanup({1});
        }
    });
    cleanup_thread.join();
    subscriber_thread.join();
    event_loop_thread.join();
}