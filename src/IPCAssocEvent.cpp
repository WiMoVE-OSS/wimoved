#include <cassert>
#include "IPCAssocEvent.h"

void IPCAssocEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_assoc(this);
}
