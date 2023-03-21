#ifndef GAFFA_IPC_EVENT_H
#define GAFFA_IPC_EVENT_H

#include <string>

#include "../Station.h"
#include "chrono"

class EventHandler;

namespace ipc {
class Event {
   public:
    explicit Event(Station station);

    virtual ~Event() = default;

    virtual void handle(EventHandler *handler) = 0;

    Station station;
    uint64_t finished_processing();

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> created_at;
};
}  // namespace ipc

#include "../EventHandler.h"

#endif  // GAFFA_IPC_EVENT_H
