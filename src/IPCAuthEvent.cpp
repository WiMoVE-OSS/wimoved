#include <cassert>
#include <utility>
#include "IPCAuthEvent.h"

void IPCAuthEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_auth(this);
}

IPCAuthEvent::IPCAuthEvent(std::string station_mac) : IPCEvent(std::move(station_mac)) {

}
