#ifndef GAFFA_IPCEVENT_H
#define GAFFA_IPCEVENT_H

#include <string>

class EventHandler;

class IPCEvent {
public:
    explicit IPCEvent(std::string station_mac);
    virtual ~IPCEvent() = default;
    virtual void handle(EventHandler *handler) = 0;
    uint32_t vni();

    std::string station_mac;
};

#include "EventHandler.h"

#endif //GAFFA_IPCEVENT_H
