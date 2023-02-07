#include <chrono>
#include <iostream>
#include <stdexcept>
#include "IPCSubscriber.h"

IPCSubscriber::IPCSubscriber(IPCQueue &queue, const std::string& iface) : socket(iface, std::chrono::seconds(1)), queue(queue) {

}

void IPCSubscriber::loop() {
    std::optional<std::string> result = socket.send_and_receive({"ATTACH"});
    if (!result.has_value()) {
        throw std::runtime_error(std::string("socket timeout in attach to hostapd"));
    }
    if (result.value() != "OK\n") {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result.value());
    }
    std::cout << "attached to hostapd" << std::endl;
    while (true) {
        std::optional<std::string> event = socket.receive();
        if (!event.has_value()) {
            std::optional<std::string> ping_result = socket.send_and_receive({"PING"});
            if (!ping_result.has_value() || ping_result.value() != "PONG\n") {
                break;
            }
        } else {
            queue.enqueue(IPCEvent(event.value()));
        }
    }
}
