#include <chrono>
#include <iostream>
#include <stdexcept>
#include "Subscriber.h"
#include "AssocEvent.h"

const std::string HOSTAPD_ASSOC_STRING = "<3>EAPOL-4WAY-HS-COMPLETED ";
const std::string HOSTAPD_AUTH_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISASSOC_STRING = "<3>AP-STA-DISCONNECTED ";

ipc::Subscriber::Subscriber(Queue &queue, const std::string& iface) : socket(iface, std::chrono::seconds(1)), queue(queue) {

}

void ipc::Subscriber::loop() {
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
                std::string station_mac = event.value().substr(HOSTAPD_ASSOC_STRING.size(), HOSTAPD_ASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
                Station station(station_mac);
                queue.enqueue(std::make_unique<AssocEvent>(std::move(station)));
            } else if (event.value().rfind(HOSTAPD_AUTH_STRING) == 0){
                std::string station_mac = event.value().substr(HOSTAPD_AUTH_STRING.size(), HOSTAPD_AUTH_STRING.size() + MAC_ADDRESS_LENGTH);
                Station station(station_mac);
                queue.enqueue(std::make_unique<AuthEvent>(std::move(station)));
            } else if (event.value().rfind(HOSTAPD_DISASSOC_STRING) == 0) {
                std::string station_mac = event.value().substr(HOSTAPD_DISASSOC_STRING.size(), HOSTAPD_DISASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
                Station station(station_mac);
                queue.enqueue(std::make_unique<DisassocEvent>(std::move(station)));
            } else {
                std::cout << "unknown event" << event.value() << std::endl;
            }
        }
    }
}
