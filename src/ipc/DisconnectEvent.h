#ifndef WIMOVED_DISCONNECTEVENT_H
#define WIMOVED_DISCONNECTEVENT_H

namespace ipc {
class DisconnectEvent;
}

#include "Event.h"

namespace ipc {
class DisconnectEvent : public Event {
   public:
    explicit DisconnectEvent(Station station);

    void handle(EventHandler *handler) override;
};
}  // namespace ipc

#endif  // WIMOVED_DISCONNECTEVENT_H
