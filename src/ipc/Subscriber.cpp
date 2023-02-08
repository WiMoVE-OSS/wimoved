#include <chrono>
#include <iostream>
#include <stdexcept>
#include "Subscriber.h"
#include "AssocEvent.h"
#include "TimeoutException.h"

const std::string HOSTAPD_ASSOC_STRING = "<3>EAPOL-4WAY-HS-COMPLETED ";
const std::string HOSTAPD_AUTH_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISASSOC_STRING = "<3>AP-STA-DISCONNECTED ";

ipc::Subscriber::Subscriber(Queue &queue, const std::string& iface) : socket(iface, std::chrono::seconds(1)), queue(queue) {

}

void ipc::Subscriber::loop(const std::future<void> &future) {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != "OK\n") {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result);
    }
    std::cout << "attached to hostapd" << std::endl;
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::string event;
        try {
            event = socket.receive();
        } catch (const ipc::TimeoutException& e) {
            try {
                std::string ping_result = socket.send_and_receive({"PING"});
                if (ping_result != "PONG\n") {
                    std::cout << "timeout in Subscriber::loop(), hostapd did not respond pong" << std::endl;
                    break;
                } else {
                    continue;
                }
            } catch (const ipc::TimeoutException& e) {
                std::cout << "timeout in Subscriber::loop(), hostapd timed out while responding to ping" << std::endl;
                break;
            }
        }
        if (event.rfind(HOSTAPD_ASSOC_STRING) == 0) {
            std::string station_mac = event.substr(HOSTAPD_ASSOC_STRING.size(), HOSTAPD_ASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            Station station(station_mac);
            queue.enqueue(std::make_unique<AssocEvent>(std::move(station)));
        } else if (event.rfind(HOSTAPD_AUTH_STRING) == 0){
            std::string station_mac = event.substr(HOSTAPD_AUTH_STRING.size(), HOSTAPD_AUTH_STRING.size() + MAC_ADDRESS_LENGTH);
            Station station(station_mac);
            queue.enqueue(std::make_unique<AuthEvent>(std::move(station)));
        } else if (event.rfind(HOSTAPD_DISASSOC_STRING) == 0) {
            std::string station_mac = event.substr(HOSTAPD_DISASSOC_STRING.size(), HOSTAPD_DISASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            Station station(station_mac);
            queue.enqueue(std::make_unique<DisassocEvent>(std::move(station)));
        } else {
            std::cout << "unknown event" << event << std::endl;
        }
    }
}
