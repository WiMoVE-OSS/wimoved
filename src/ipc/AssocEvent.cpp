#include "AssocEvent.h"

#include <cassert>
#include <utility>

void ipc::AssocEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_assoc(this);
}

ipc::AssocEvent::AssocEvent(Station station) : Event(std::move(station)) {}