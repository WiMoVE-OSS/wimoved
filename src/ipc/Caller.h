#ifndef GAFFA_CALLER_H
#define GAFFA_CALLER_H


#include "Socket.h"
#include "../Station.h"

namespace ipc {
    class Caller {
    public:
        explicit Caller(const std::string &socket_path);

        uint32_t vlan_for_station(const std::string &station_mac);

        std::vector<Station> connected_stations();

    private:
        Socket socket;
    };
}


#endif //GAFFA_CALLER_H
