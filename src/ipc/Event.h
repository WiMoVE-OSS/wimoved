#ifndef GAFFA_EVENT_H
#define GAFFA_EVENT_H

#include <string>
#include "../Station.h"

class EventHandler;

namespace ipc {
    class Event {
    public:
        explicit Event(Station station);

        virtual ~Event() = default;

        virtual void handle(EventHandler *handler) = 0;

        Station station;
    };
}

#include "../EventHandler.h"

#endif //GAFFA_EVENT_H
