#ifndef GAFFA_IPCASSOCEVENT_H
#define GAFFA_IPCASSOCEVENT_H

class IPCAssocEvent;

#include "IPCEvent.h"

class IPCAssocEvent : public IPCEvent {
    virtual void handle(EventHandler *handler) override;
};


#endif //GAFFA_IPCASSOCEVENT_H
