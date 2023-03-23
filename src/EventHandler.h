#ifndef WIMOVED_EVENTHANDLER_H
#define WIMOVED_EVENTHANDLER_H

#include "ipc/ConnectEvent.h"
#include "ipc/DisconnectEvent.h"

class EventHandler {
   public:
    virtual ~EventHandler() = default;
    virtual void handle_connect(ipc::ConnectEvent* event) = 0;
    virtual void handle_disconnect(ipc::DisconnectEvent* event) = 0;
};

#endif  // WIMOVED_EVENTHANDLER_H
