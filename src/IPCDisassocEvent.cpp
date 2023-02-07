#include "IPCDisassocEvent.h"

#include <utility>

void IPCDisassocEvent::handle(EventHandler *handler) {
    handler->handle_disassoc(this);
}

IPCDisassocEvent::IPCDisassocEvent(Station station) : IPCEvent(std::move(station)) {
}
