#include "IPCDisassocEvent.h"

#include <utility>

void IPCDisassocEvent::handle(EventHandler *handler) {
    handler->handle_disassoc(this);
}

IPCDisassocEvent::IPCDisassocEvent(std::string station_mac) : IPCEvent(std::move(station_mac)) {
}
