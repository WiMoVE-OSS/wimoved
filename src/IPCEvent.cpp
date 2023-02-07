#include "IPCEvent.h"

#include <utility>

const std::string& IPCEvent::content() const {
    return text;
}

IPCEvent::IPCEvent(std::string text) : text(std::move(text)) {
}

void IPCEvent::handle(EventHandler *handler) {
    handler->handle_assoc(this);
}
