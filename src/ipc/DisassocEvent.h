#ifndef GAFFA_DISASSOCEVENT_H
#define GAFFA_DISASSOCEVENT_H

namespace ipc {
class DisassocEvent;
}

#include "Event.h"

namespace ipc {
class DisassocEvent : public Event {
   public:
    explicit DisassocEvent(Station station);

    void handle(EventHandler *handler) override;
};
}  // namespace ipc

#endif  // GAFFA_DISASSOCEVENT_H
