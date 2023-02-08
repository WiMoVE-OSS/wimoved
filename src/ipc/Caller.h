#ifndef GAFFA_CALLER_H
#define GAFFA_CALLER_H


#include "Socket.h"

namespace ipc {
    class Caller {
    public:
        explicit Caller(const std::string &iface);

        uint32_t vlan_for_station(const std::string &station_mac);

        std::vector<std::string> connected_stations();

    private:
        Socket socket;
    };
}


#endif //GAFFA_CALLER_H
