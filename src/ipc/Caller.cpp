#include <chrono>
#include <stdexcept>
#include <sstream>
#include <thread>
#include "Caller.h"
#include "../Station.h"

const std::string VLAN_ID_PREFIX = "vlan_id=";

ipc::Caller::Caller(const std::string &socket_path) : socket(socket_path, std::chrono::seconds(1)) {

}

uint32_t ipc::Caller::vlan_for_station(const std::string &station_mac) {
    for (int i = 0; i < 10; i++) {
        std::istringstream stream(socket.send_and_receive({"STA", station_mac}));
        std::string line;
        while (std::getline(stream, line)) {
            if (line.rfind(VLAN_ID_PREFIX) == 0) {
                return std::stol(line.substr(VLAN_ID_PREFIX.size(), line.size()));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    throw std::runtime_error("no vlan_id attribute found for station");
}

std::vector<Station> ipc::Caller::connected_stations() {
    std::vector<Station> stations;
    std::string ipc_result = socket.send_and_receive({"STA-FIRST"});
    while (!ipc_result.empty()) {
        if (ipc_result !="FAIL\n") {
            stations.emplace_back(ipc_result.substr(0, MAC_ADDRESS_LENGTH));
        }
        ipc_result = socket.send_and_receive({"STA-NEXT", stations[stations.size()-1].mac});
    }
    return stations;
}
