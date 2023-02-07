#ifndef GAFFA_IPCASSOCEVENT_H
#define GAFFA_IPCASSOCEVENT_H

class IPCAssocEvent;

#include "IPCEvent.h"

class IPCAssocEvent : public IPCEvent {
public:
    explicit IPCAssocEvent(std::string station_mac);
    virtual void handle(EventHandler *handler) override;

    uint32_t vlan_id;
};


#endif //GAFFA_IPCASSOCEVENT_H
