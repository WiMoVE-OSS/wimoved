#include "IPCDisassocEvent.h"

void IPCDisassocEvent::handle(EventHandler *handler) {
    handler->handle_disassoc(this);
}
