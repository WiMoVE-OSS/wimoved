#ifndef WIMOVED_CALLER_H
#define WIMOVED_CALLER_H

#include "../Station.h"
#include "Socket.h"

namespace ipc {
class Caller {
   public:
    Caller();

    uint32_t vlan_for_station(const std::string &station_mac);

    std::vector<Station> connected_stations();

   private:
    Socket socket;
};
}  // namespace ipc

#endif  // WIMOVED_CALLER_H
