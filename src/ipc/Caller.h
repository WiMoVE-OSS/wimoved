#ifndef WIMOVED_CALLER_H
#define WIMOVED_CALLER_H

#include <unordered_map>

#include "../Station.h"
#include "Socket.h"

namespace ipc {
class Caller {
   public:
    Caller();

    uint32_t vlan_for_station(const Station& station);

    std::vector<Station> connected_stations();
    void deauth_station(const Station& station);

   private:
    std::unordered_map<std::string, Socket> sockets;
    Socket& get_socket(std::string name);
};
}  // namespace ipc

#endif  // WIMOVED_CALLER_H
