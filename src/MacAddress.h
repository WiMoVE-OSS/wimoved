#ifndef WIMOVED_MACADDRESS_H
#define WIMOVED_MACADDRESS_H

#include <array>
#include <cstdint>
#include <string>

const int MAC_NUM_BYTES = 6;

class MacAddress {
   public:
    explicit MacAddress(std::string serialized);
    std::string string() const;
    uint64_t number() const;

   private:
    std::array<uint8_t, MAC_NUM_BYTES> data;
    std::string serialized;
};

#endif  // WIMOVED_MACADDRESS_H
