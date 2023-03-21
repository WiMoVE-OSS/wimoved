#ifndef WIMOVED_EVENTHANDLER_H
#define WIMOVED_EVENTHANDLER_H

#include "ipc/AssocEvent.h"
#include "ipc/AuthEvent.h"
#include "ipc/DisassocEvent.h"

class EventHandler {
   public:
    virtual void handle_assoc(ipc::AssocEvent* event) = 0;
    virtual void handle_auth(ipc::AuthEvent* event) = 0;
    virtual void handle_disassoc(ipc::DisassocEvent* event) = 0;
};

#endif  // WIMOVED_EVENTHANDLER_H
