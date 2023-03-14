#ifndef GAFFA_NL_EVENT_H
#define GAFFA_NL_EVENT_H

#include <string>

namespace nl {
class Event {
   public:
    Event(std::string interface_name, uint32_t ifindex);

    std::string interface_name;
    uint32_t ifindex;
};
}  // namespace nl

#endif
