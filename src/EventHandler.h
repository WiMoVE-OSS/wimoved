#ifndef GAFFA_EVENTHANDLER_H
#define GAFFA_EVENTHANDLER_H

class EventHandler;

#include "IPCEvent.h"

class EventHandler {
public:
    virtual void handle_assoc(const IPCEvent* event) = 0;
    virtual void handle_auth(const IPCEvent* event) = 0;
    virtual void handle_disassoc(const IPCEvent* event) = 0;
};

#endif //GAFFA_EVENTHANDLER_H
