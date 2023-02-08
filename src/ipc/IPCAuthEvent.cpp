#include <cassert>
#include <utility>
#include "IPCAuthEvent.h"

void IPCAuthEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_auth(this);
}

IPCAuthEvent::IPCAuthEvent(Station station) : IPCEvent(std::move(station)) {

}
