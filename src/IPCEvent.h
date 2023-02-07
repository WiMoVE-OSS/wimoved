#ifndef GAFFA_IPCEVENT_H
#define GAFFA_IPCEVENT_H

#include <string>
#include "Station.h"

class EventHandler;

class IPCEvent {
public:
    explicit IPCEvent(Station station);
    virtual ~IPCEvent() = default;
    virtual void handle(EventHandler *handler) = 0;
    Station station;
};

#include "EventHandler.h"

#endif //GAFFA_IPCEVENT_H
