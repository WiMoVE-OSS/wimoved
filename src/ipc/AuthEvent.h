#ifndef WIMOVED_AUTHEVENT_H
#define WIMOVED_AUTHEVENT_H

namespace ipc {
class AuthEvent;
}

#include "Event.h"

namespace ipc {
class AuthEvent : public Event {
   public:
    explicit AuthEvent(Station station);

    void handle(EventHandler *handler) override;
};
}  // namespace ipc

#endif  // WIMOVED_AUTHEVENT_H
