#include <chrono>
#include <iostream>
#include <stdexcept>
#include "IPCSubscriber.h"
#include "IPCAssocEvent.h"

const std::string HOSTAPD_ASSOC_STRING = "<3>EAPOL-4WAY-HS-COMPLETED ";
const std::string HOSTAPD_AUTH_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISASSOC_STRING = "<3>AP-STA-DISCONNECTED ";

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
            if (event.value().rfind(HOSTAPD_ASSOC_STRING) == 0) {
                queue.enqueue(std::make_unique<IPCAssocEvent>());
            } else if (event.value().rfind(HOSTAPD_AUTH_STRING) == 0){
                queue.enqueue(std::make_unique<IPCAuthEvent>());
            } else if (event.value().rfind(HOSTAPD_DISASSOC_STRING) == 0) {
                queue.enqueue(std::make_unique<IPCDisassocEvent>());
            } else {
                std::cout << "unknown event" << event.value() << std::endl;
            }
        }
    }
}
