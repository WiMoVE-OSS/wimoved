#ifndef GAFFA_STATION_H
#define GAFFA_STATION_H

#include <optional>
#include <string>

class Station {
   public:
    Station(std::string mac, uint32_t vlan_id);
    uint32_t vni() const;

    uint32_t vlan_id;
    std::string mac;

    std::string vlan_interface_name() const;
};

#endif  // GAFFA_STATION_H
