#include "Subscriber.h"

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "../logging/loginit.h"
#include "AssocEvent.h"

const std::string HOSTAPD_ASSOC_STRING = "<3>EAPOL-4WAY-HS-COMPLETED ";
const std::string HOSTAPD_AUTH_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISASSOC_STRING = "<3>AP-STA-DISCONNECTED ";

ipc::Subscriber::Subscriber(SynchronizedQueue<Event>& queue, const std::chrono::duration<int>& timeout)
    : socket(timeout), queue(queue) {}

void ipc::Subscriber::loop(const std::future<void>& future) {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != "OK\n") {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result);
    }
    GAFFALOG(INFO) << "attached to hostapd";
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::string event;
        try {
            event = socket.receive();
        } catch (const TimeoutException& e) {
            try {
                std::string ping_result = socket.send_and_receive({"PING"});
                if (ping_result != "PONG\n") {
                    GAFFALOG(FATAL) << "timeout in Subscriber::loop_ipc_queue(), hostapd did not respond pong: "
                                    << ping_result;
                    break;
                } else {
                    continue;
                }
            } catch (const TimeoutException& e) {
                GAFFALOG(FATAL)
                    << "timeout in Subscriber::loop_ipc_queue(), hostapd timed out while responding to ping";
                break;
            }
        }
        if (event.rfind(HOSTAPD_ASSOC_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_ASSOC_STRING.size(), HOSTAPD_ASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<AssocEvent>(std::move(station)));
            } catch (const std::runtime_error& e) {
                GAFFALOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_AUTH_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_AUTH_STRING.size(), HOSTAPD_AUTH_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<AuthEvent>(std::move(station)));
            } catch (const std::runtime_error& e) {
                GAFFALOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_DISASSOC_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_DISASSOC_STRING.size(), HOSTAPD_DISASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<DisassocEvent>(std::move(station)));
            } catch (const std::runtime_error& e) {
                GAFFALOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else {
            GAFFALOG(DEBUG) << "Received unknown event" << event;
        }
    }
}
