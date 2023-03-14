#ifndef WM_CALLER_H
#define WM_CALLER_H

#include "../Station.h"
#include "Socket.h"

namespace ipc {
class Caller {
   public:
    Caller();

    std::vector<Station> connected_stations();

   private:
    Socket socket;
};
}  // namespace ipc

#endif  // WM_CALLER_H
