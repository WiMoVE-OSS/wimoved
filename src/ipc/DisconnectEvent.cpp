#include "DisconnectEvent.h"

#include <cassert>
#include <utility>

void ipc::DisconnectEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_disconnect(this);
}

ipc::DisconnectEvent::DisconnectEvent(Station station) : Event(std::move(station)) {}
