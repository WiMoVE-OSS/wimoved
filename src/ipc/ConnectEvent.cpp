#include "ConnectEvent.h"

#include <cassert>
#include <utility>

void ipc::ConnectEvent::handle(EventHandler *handler) {
    handler->handle_connect(this);
}

ipc::ConnectEvent::ConnectEvent(Station station) : ipc::Event(std::move(station)) {}
