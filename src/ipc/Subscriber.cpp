#include "Subscriber.h"

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "../logging/loginit.h"
#include "../metrics/MetricsManager.h"
#include "AssocEvent.h"

const std::string HOSTAPD_PONG = "PONG\n";
const std::string HOSTAPD_ASSOC_STRING = "<3>EAPOL-4WAY-HS-COMPLETED ";
const std::string HOSTAPD_AUTH_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISASSOC_STRING = "<3>AP-STA-DISCONNECTED ";

ipc::Subscriber::Subscriber(SynchronizedQueue<Event>& queue, const std::chrono::duration<int>& timeout)
    : socket(timeout),
      queue(queue),
      hostapd_association_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("assoc")),
      hostapd_disassociation_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("disassoc")),
      hostapd_authentication_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("auth")),
      hostapd_unknown_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("unknown")) {}

void ipc::Subscriber::loop(const std::future<void>& future) {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != Socket::HOSTAPD_OK) {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result);
    }
    WMLOG(INFO) << "attached to hostapd";
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::string event;
        try {
            event = socket.receive();
        } catch (const TimeoutException& e) {
            try {
                event = socket.send_and_receive({"PING"});
            } catch (const TimeoutException& e) {
                WMLOG(FATAL) << "timeout in Subscriber::loop_ipc_queue(), hostapd timed out while responding to ping";
                break;
            }
        }
        // This is a PONG in response to a previous PING where another event came first, ignore.
        if (event == HOSTAPD_PONG) {
            continue;
        }

        if (event.rfind(HOSTAPD_ASSOC_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_ASSOC_STRING.size(), HOSTAPD_ASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<AssocEvent>(std::move(station)));
                hostapd_association_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_AUTH_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_AUTH_STRING.size(), HOSTAPD_AUTH_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<AuthEvent>(std::move(station)));
                hostapd_authentication_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_DISASSOC_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_DISASSOC_STRING.size(), HOSTAPD_DISASSOC_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<DisassocEvent>(std::move(station)));
                hostapd_disassociation_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create station: " << e.what();
            }
        } else {
            hostapd_unknown_counter.Increment();
            WMLOG(DEBUG) << "Received unknown event" << event;
        }
    }
}
