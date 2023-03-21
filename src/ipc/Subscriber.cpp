#include "Subscriber.h"

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "../logging/loginit.h"
#include "../metrics/MetricsManager.h"

const std::string HOSTAPD_CONNECT_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISCONNECT_STRING = "<3>AP-STA-DISCONNECTED ";
const std::string HOSTAPD_PONG = "PONG\n";

ipc::Subscriber::Subscriber(SynchronizedQueue<Event>& queue, const std::chrono::duration<int>& timeout)
    : socket(timeout),
      queue(queue),
      hostapd_disconnect_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("disconnect")),
      hostapd_connect_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("connect")),
      hostapd_unknown_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("unknown")) {}

void ipc::Subscriber::loop(const std::future<void>& future) {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != "OK\n") {
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

        if (event.rfind(HOSTAPD_CONNECT_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_CONNECT_STRING.size(), HOSTAPD_CONNECT_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<ConnectEvent>(std::move(station)));
                hostapd_connect_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create event: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_DISCONNECT_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_DISCONNECT_STRING.size(), HOSTAPD_DISCONNECT_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(station_mac);
                queue.enqueue(std::make_unique<DisconnectEvent>(std::move(station)));
                hostapd_disconnect_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create event: " << e.what();
            }
        } else {
            hostapd_unknown_counter.Increment();
            WMLOG(DEBUG) << "Received unknown event " << event;
        }
    }
}
