#ifndef WM_STATION_H
#define WM_STATION_H

#include <chrono>
#include <optional>
#include <string>

static const std::string VLAN_INTERFACE_PREFIX = "vlan";

class Station {
   public:
    Station(std::string mac, uint32_t vlan_id);
    uint32_t vni() const;

    uint32_t vlan_id;
    std::string mac;

    std::string vlan_interface_name() const;
    uint64_t finished_processing();

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> created_at;
};

#endif  // WM_STATION_H
