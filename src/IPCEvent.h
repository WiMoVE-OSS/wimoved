#ifndef GAFFA_IPCEVENT_H
#define GAFFA_IPCEVENT_H

class IPCEvent;

#include <string>
#include "EventHandler.h"

class IPCEvent {
private:
    std::string text;
public:
    IPCEvent(std::string text);
    void handle(EventHandler *handler);
    const std::string& content() const;
};


#endif //GAFFA_IPCEVENT_H
