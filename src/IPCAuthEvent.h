#ifndef GAFFA_IPCAUTHEVENT_H
#define GAFFA_IPCAUTHEVENT_H

class IPCAuthEvent;

#include "IPCEvent.h"

class IPCAuthEvent : public IPCEvent {
public:
    explicit IPCAuthEvent(Station station);
    void handle(EventHandler *handler) override;
};


#endif //GAFFA_IPCAUTHEVENT_H
