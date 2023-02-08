#include "Station.h"

#include <utility>

Station::Station(std::string mac) : vlan_id(std::nullopt), mac(std::move(mac)) {

}

uint32_t Station::vni() const {
    return 1;
}
