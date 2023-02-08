#include <cassert>
#include <utility>
#include "AuthEvent.h"

void ipc::AuthEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_auth(this);
}

ipc::AuthEvent::AuthEvent(Station station) : ipc::Event(std::move(station)) {

}
