#include "IPCEvent.h"

#include <utility>

const std::string& IPCEvent::content() const {
    return text;
}

IPCEvent::IPCEvent(std::string text) : text(std::move(text)) {
}
