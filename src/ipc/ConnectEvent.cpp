#include "ConnectEvent.h"

#include <cassert>
#include <utility>

void ipc::ConnectEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_connect(this);
}

ipc::ConnectEvent::ConnectEvent(Station station) : ipc::Event(std::move(station)) {}
