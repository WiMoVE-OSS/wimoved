#include <iostream>
#include <thread>
#include <future>
#include <csignal>
#include "BridgePerVxlanRenderer.h"
#include "ipc/Subscriber.h"
#include "EventLoop.h"

std::vector<std::promise<void>> promises(3);
bool promises_resolved = false;
std::mutex promises_mutex;

void handle_signal(int signal) {
    std::lock_guard g(promises_mutex);
    if (promises_resolved) {
        return;
    }
    promises_resolved = true;
    for (std::promise<void>& promise : promises) {
        promise.set_value();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    std::string iface = argv[1];
    ipc::Queue queue;
    BridgePerVxlanRenderer renderer;

    std::thread subscriber_thread([&queue, &iface](){
        ipc::Subscriber(queue, iface).loop(promises[0].get_future());
    });
    std::thread event_loop_thread([&renderer, &queue, &iface]() {
        EventLoop(renderer, queue, iface).loop(promises[1].get_future());
    });
    std::thread cleanup_thread([&renderer, &iface]() {
        std::future<void> future = promises[2].get_future();
        ipc::Caller caller(iface);
        while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            renderer.cleanup([&caller]() {
                return caller.connected_stations();
            });
        }
    });
    subscriber_thread.join();
    event_loop_thread.join();
    cleanup_thread.join();
}