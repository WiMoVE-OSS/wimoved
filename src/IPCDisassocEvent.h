#ifndef GAFFA_IPCDISASSOCEVENT_H
#define GAFFA_IPCDISASSOCEVENT_H

class IPCDisassocEvent;

#include "IPCEvent.h"

class IPCDisassocEvent : public IPCEvent {
public:
    explicit IPCDisassocEvent(std::string station_mac);
    virtual void handle(EventHandler *handler) override;
};


#endif //GAFFA_IPCDISASSOCEVENT_H
