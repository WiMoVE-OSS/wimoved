#include "Caller.h"

#include <chrono>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "../Station.h"

ipc::Caller::Caller() : socket(std::chrono::seconds(1)) {}

std::vector<Station> ipc::Caller::connected_stations() {
    std::vector<Station> stations;
    std::string ipc_result = socket.send_and_receive({"STA-FIRST"});
    while (!ipc_result.empty()) {
        if (ipc_result != "FAIL\n") {
            stations.emplace_back(ipc_result.substr(0, MAC_ADDRESS_LENGTH), 0);
        }
        ipc_result = socket.send_and_receive({"STA-NEXT", stations[stations.size() - 1].mac});
    }
    return stations;
}
