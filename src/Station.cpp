#include "Station.h"

#include <algorithm>
#include <regex>
#include <utility>

#include "Configuration.h"

const auto MAC_REGEX = std::regex("^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$");

Station::Station(std::string mac, uint32_t vlan_id) : vlan_id(vlan_id), mac(std::move(mac)) {
    if (!std::regex_match(this->mac, MAC_REGEX)) {
        throw std::runtime_error("MAC address not valid");
    }
}

uint32_t Station::vni() const {
    std::string sanitized_mac = mac;
    // Remove all colons from MAC address
    sanitized_mac.erase(std::remove(sanitized_mac.begin(), sanitized_mac.end(), ':'), sanitized_mac.end());

    uint64_t x = std::stoull(sanitized_mac, nullptr, 16);

    return x % Configuration::get_instance().max_vni;
}

std::string Station::vlan_interface_name() const { return VLAN_INTERFACE_PREFIX + std::to_string(vlan_id); }