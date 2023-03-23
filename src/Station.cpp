#include "Station.h"

#include <utility>

#include "Configuration.h"
#include "MacAddress.h"

Station::Station(std::string sockname, MacAddress mac)
    : vlan_id(std::nullopt), mac(std::move(mac)), sockname(std::move(sockname)) {}

uint32_t Station::vni() const { return mac.number() % Configuration::get_instance().max_vni; }

std::string Station::vlan_interface_name() const { return "vlan" + std::to_string(vlan_id.value_or(0)); }

void Station::log(el::base::type::ostream_t& os) const { os << mac.string(); }
