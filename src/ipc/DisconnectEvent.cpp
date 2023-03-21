#include "DisconnectEvent.h"

#include <utility>

void ipc::DisconnectEvent::handle(EventHandler *handler) { handler->handle_disconnect(this); }

ipc::DisconnectEvent::DisconnectEvent(Station station) : Event(std::move(station)) {}
