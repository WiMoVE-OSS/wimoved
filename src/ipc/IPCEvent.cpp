#include "IPCEvent.h"

#include <utility>

IPCEvent::IPCEvent(Station station) : station(std::move(station)) {

}