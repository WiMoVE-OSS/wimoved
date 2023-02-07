#ifndef GAFFA_EVENTHANDLER_H
#define GAFFA_EVENTHANDLER_H

#include "IPCAssocEvent.h"
#include "IPCAuthEvent.h"
#include "IPCDisassocEvent.h"

class EventHandler {
public:
    virtual void handle_assoc(const IPCAssocEvent* event) = 0;
    virtual void handle_auth(const IPCAuthEvent* event) = 0;
    virtual void handle_disassoc(const IPCDisassocEvent* event) = 0;
};

#endif //GAFFA_EVENTHANDLER_H
