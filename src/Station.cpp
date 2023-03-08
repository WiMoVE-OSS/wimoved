#include "Station.h"

#include <algorithm>
#include <regex>
#include <utility>

const auto NUM_VNIS = 20;
const auto MAC_REGEX = std::regex("^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$");

Station::Station(const std::string& mac) : vlan_id(std::nullopt), mac(mac) {
    if (!std::regex_match(mac, MAC_REGEX)) {
        throw std::runtime_error("MAC address not valid");
    }
}

uint32_t Station::vni() const {
    std::string sanitized_mac = mac;
    // Remove all colons from MAC address
    sanitized_mac.erase(std::remove(sanitized_mac.begin(), sanitized_mac.end(), ':'), sanitized_mac.end());

    uint64_t x = std::stoull(sanitized_mac, nullptr, 16);

    return x % NUM_VNIS;
}

std::string Station::vlan_interface_name() const { return "vlan" + std::to_string(vlan_id.value_or(0)); }