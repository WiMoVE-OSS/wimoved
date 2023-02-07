#include "IPCEvent.h"

#include <utility>

IPCEvent::IPCEvent(std::string station_mac) : station_mac(std::move(station_mac)) {

}

uint32_t IPCEvent::vni() {
    // TODO: implement proper VNI calculation
    return 1;
}
