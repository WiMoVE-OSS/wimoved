#ifndef GAFFA_IPCEVENT_H
#define GAFFA_IPCEVENT_H


#include <string>

class IPCEvent {
private:
    std::string text;
public:
    IPCEvent(std::string text);
    const std::string& content() const;
};


#endif //GAFFA_IPCEVENT_H
