#ifndef GAFFA_IPCEVENT_H
#define GAFFA_IPCEVENT_H

#include <string>

class EventHandler;

class IPCEvent {
public:
    virtual void handle(EventHandler *handler) = 0;
    virtual ~IPCEvent() = default;
};

#include "EventHandler.h"

#endif //GAFFA_IPCEVENT_H
