#ifndef GAFFA_STATION_H
#define GAFFA_STATION_H


#include <string>
#include <optional>

class Station {
public:
    Station(std::string mac);
    uint32_t vni();
    std::optional<uint32_t> vlan_id;
    std::string mac;
};


#endif //GAFFA_STATION_H
