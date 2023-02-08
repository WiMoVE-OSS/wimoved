#include "DisassocEvent.h"

#include <utility>

void ipc::DisassocEvent::handle(EventHandler *handler) {
    handler->handle_disassoc(this);
}

ipc::DisassocEvent::DisassocEvent(Station station) : Event(std::move(station)) {
}
