#ifndef GAFFA_EVENTHANDLER_H
#define GAFFA_EVENTHANDLER_H

#include "ipc/IPCAssocEvent.h"
#include "ipc/IPCAuthEvent.h"
#include "ipc/IPCDisassocEvent.h"

class EventHandler {
public:
    virtual void handle_assoc(IPCAssocEvent* event) = 0;
    virtual void handle_auth(IPCAuthEvent* event) = 0;
    virtual void handle_disassoc(IPCDisassocEvent* event) = 0;
};

#endif //GAFFA_EVENTHANDLER_H
