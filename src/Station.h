#ifndef WIMOVED_STATION_H
#define WIMOVED_STATION_H

#include <optional>
#include <string>

class Station {
   public:
    Station(std::string mac);
    uint32_t vni() const;
    std::optional<uint32_t> vlan_id;
    std::string mac;

    std::string vlan_interface_name() const;
};

#endif  // WIMOVED_STATION_H
