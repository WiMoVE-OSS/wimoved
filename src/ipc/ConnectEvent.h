#ifndef WIMOVED_CONNECTEVENT_H
#define WIMOVED_CONNECTEVENT_H

namespace ipc {
class ConnectEvent;
}

#include "Event.h"

namespace ipc {
class ConnectEvent : public Event {
   public:
    explicit ConnectEvent(Station station);

    void handle(EventHandler *handler) override;
};
}  // namespace ipc

#endif  // WIMOVED_CONNECTEVENT_H
