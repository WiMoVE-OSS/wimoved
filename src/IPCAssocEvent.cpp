#include <cassert>
#include <utility>
#include "IPCAssocEvent.h"

void IPCAssocEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_assoc(this);
}

IPCAssocEvent::IPCAssocEvent(std::string station_mac) : IPCEvent(std::move(station_mac)) {

}