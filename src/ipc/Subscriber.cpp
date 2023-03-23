#include "Subscriber.h"

#include <chrono>
#include <stdexcept>

#include "../Configuration.h"
#include "../metrics/MetricsManager.h"
#include "logging/loginit.h"

const std::string HOSTAPD_CONNECT_STRING = "<3>AP-STA-CONNECTED ";
const std::string HOSTAPD_DISCONNECT_STRING = "<3>AP-STA-DISCONNECTED ";
const std::string HOSTAPD_PONG = "PONG\n";
const std::string HOSTAPD_IFPREFIX = "IFNAME=";
const std::string HOSTAPD_GLOBAL = "global";

static std::pair<std::string, std::string> split_at_first_space(const std::string& line) {
    auto position = line.find(' ');
    if (position == std::string::npos) {
        throw std::runtime_error("Could not parse event line: " + line);
    }
    return {line.substr(0, position), line.substr(position + 1)};
}

static std::string get_ifname(const std::string& eventprefix) {
    if (eventprefix.rfind(HOSTAPD_IFPREFIX) != 0) {
        throw std::runtime_error("interface name could not be parsed from event prefix: " + eventprefix);
    }
    return eventprefix.substr(HOSTAPD_IFPREFIX.size());
}

ipc::Subscriber::Subscriber(SynchronizedQueue<Event>& queue, const std::chrono::duration<int>& timeout)
    : socket(timeout, HOSTAPD_GLOBAL),
      queue(queue),
      hostapd_disconnect_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("disconnect")),
      hostapd_connect_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("connect")),
      hostapd_unknown_counter(MetricsManager::get_instance().get_hostapd_counter_for_type("unknown")) {}

void ipc::Subscriber::loop(const std::future<void>& future) {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != Socket::HOSTAPD_OK) {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result);
    }
    WMLOG(INFO) << "attached to hostapd";
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::string line;
        try {
            line = socket.receive();
        } catch (const TimeoutException& e) {
            try {
                line = socket.send_and_receive({"PING"});
            } catch (const TimeoutException& e) {
                WMLOG(FATAL) << "timeout in Subscriber::loop_ipc_queue(), hostapd timed out while responding to ping";
                break;
            }
        }
        // This is a PONG in response to a previous PING where another event came first, ignore.
        if (line == HOSTAPD_PONG) {
            continue;
        }

        // Split up a hostAPD global event in the form IFNAME=wlan1-1 <3>AP-STA-CONNECTED 12:34:56:78:90:ab
        auto [ifname_part, event] = split_at_first_space(line);
        std::string sockname = get_ifname(ifname_part);

        std::vector<std::string> interface_names = Configuration::get_instance().socknames;
        if (std::find(interface_names.begin(), interface_names.end(), sockname) == interface_names.end()) {
            WMLOG(DEBUG) << "Received event on interface that is not configured " << sockname << ": " << event;
            continue;
        }

        if (event.rfind(HOSTAPD_CONNECT_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_CONNECT_STRING.size(), HOSTAPD_CONNECT_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(sockname, station_mac);
                queue.enqueue(std::make_unique<ConnectEvent>(std::move(station)));
                hostapd_connect_counter.Increment();
            } catch (const std::runtime_error& e) {
                WMLOG(ERROR) << "Unable to create event: " << e.what();
            }
        } else if (event.rfind(HOSTAPD_DISCONNECT_STRING) == 0) {
            std::string station_mac =
                event.substr(HOSTAPD_DISCONNECT_STRING.size(), HOSTAPD_DISCONNECT_STRING.size() + MAC_ADDRESS_LENGTH);
            try {
                Station station(sockname, station_mac);
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