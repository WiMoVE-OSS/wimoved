#ifndef WIMOVED_STATION_H
#define WIMOVED_STATION_H

#include <optional>
#include <string>

#include "MacAddress.h"
#include "logging/easylogging++.h"

class Station : public el::Loggable {
   public:
    Station(std::string sockname, MacAddress mac);
    uint32_t vni() const;
    std::optional<uint32_t> vlan_id;
    MacAddress mac;
    std::string sockname;

    std::string vlan_interface_name() const;

    void log(el::base::type::ostream_t& os) const override;
};

#endif  // WIMOVED_STATION_H
