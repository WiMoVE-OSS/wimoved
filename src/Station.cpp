#include "Station.h"

#include <utility>

#include "Configuration.h"
#include "MacAddress.h"
#include "ipc/Caller.h"

Station::Station(std::string sockname, MacAddress mac)
    : vlan_id(std::nullopt), mac(std::move(mac)), sockname(std::move(sockname)) {}

uint32_t Station::vni() const {
    // Use username as VNI, throws error if it cannot be parsed.
    return std::stol(user);
}

std::string Station::vlan_interface_name() const { return "vlan" + std::to_string(vlan_id.value_or(0)); }

void Station::log(el::base::type::ostream_t& os) const { os << mac.string(); }
