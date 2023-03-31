#include "MacAddress.h"

#include <regex>

const auto MAC_REGEX = std::regex("^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$");
const int MAC_OCTET_BASE = 16;
const int BITS_PER_BYTE = 8;

MacAddress::MacAddress(std::string serialized) : data(), serialized(serialized) {
    if (!std::regex_match(serialized, MAC_REGEX)) {
        throw std::runtime_error("MAC address not valid");
    }
    serialized.erase(std::remove(serialized.begin(), serialized.end(), ':'), serialized.end());
    for (size_t i = 0; i < MAC_NUM_BYTES; i++) {
        data[i] = std::strtoul(serialized.substr(2 * i, 2).c_str(), nullptr, MAC_OCTET_BASE);
    }
}
std::string MacAddress::string() const { return serialized; }
uint64_t MacAddress::number() const {
    uint64_t res = 0;
    for (size_t i = 0; i < MAC_NUM_BYTES; i++) {
        res <<= BITS_PER_BYTE;
        res = res | data[i];
    }
    return res;
}