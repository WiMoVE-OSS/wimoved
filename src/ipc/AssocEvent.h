#ifndef WIMOVED_ASSOCEVENT_H
#define WIMOVED_ASSOCEVENT_H

namespace ipc {
class AssocEvent;
}

#include "Event.h"

namespace ipc {
class AssocEvent : public Event {
   public:
    explicit AssocEvent(Station station);

    virtual void handle(EventHandler *handler) override;
};
}  // namespace ipc

#endif  // WIMOVED_ASSOCEVENT_H
