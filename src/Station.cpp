#include "Station.h"

#include <utility>
#include <algorithm>
#include <regex>

const auto NUM_VNIS = 20;
const auto MAC_REGEX = std::regex("^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$");

Station::Station(std::string mac) : vlan_id(std::nullopt) {
    if(!std::regex_match(mac, MAC_REGEX)) {
        throw new std::runtime_error("MAC address not valid");
    }
    this->mac = mac;
}

uint32_t Station::vni() const {

    std::string sanitized_mac = mac;
    // Remove all colons from MAC address
    sanitized_mac.erase(std::remove(sanitized_mac.begin(), sanitized_mac.end(), ':'), sanitized_mac.end());

    uint64_t x = std::stoull(sanitized_mac, nullptr, 16);

    return x % NUM_VNIS;
}
