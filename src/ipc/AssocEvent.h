#ifndef GAFFA_ASSOCEVENT_H
#define GAFFA_ASSOCEVENT_H

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
}


#endif //GAFFA_ASSOCEVENT_H
