#include <iostream>
#include <thread>
#include <future>
#include <csignal>
#include "BridgePerVxlanRenderer.h"
#include "ipc/Subscriber.h"
#include "EventLoop.h"
#include "logging/easylogging++.h"
#include "ConfigParser.h"

#define ELPP_THREAD_SAFE 1
INITIALIZE_EASYLOGGINGPP


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
    LOG(INFO) << "Welcome to Gaffa!";
    std::string config_path = "/etc/gaffa/config";
    if (argc >= 2) {
        config_path = argv[1];
    }
    ConfigParser parser(config_path);

    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    ipc::Queue queue;
    BridgePerVxlanRenderer renderer;
    std::string socket_path = parser.get_config_option("hapd_sock");

    std::thread subscriber_thread([&queue, &socket_path](){
        ipc::Subscriber(queue, socket_path).loop(promises[0].get_future());
    });
    std::thread event_loop_thread([&renderer, &queue, &socket_path]() {
        EventLoop(renderer, queue, socket_path).loop(promises[1].get_future());
    });
    std::thread cleanup_thread([&renderer, &socket_path]() {
        std::future<void> future = promises[2].get_future();
        ipc::Caller caller(socket_path);
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