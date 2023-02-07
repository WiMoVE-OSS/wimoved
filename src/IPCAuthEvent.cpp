#include <cassert>
#include "IPCAuthEvent.h"

void IPCAuthEvent::handle(EventHandler *handler) {
    assert(handler != nullptr);
    handler->handle_auth(this);
}
