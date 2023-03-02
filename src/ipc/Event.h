#ifndef GAFFA_IPC_EVENT_H
#define GAFFA_IPC_EVENT_H

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
}  // namespace ipc

#include "../EventHandler.h"

#endif  // GAFFA_IPC_EVENT_H
