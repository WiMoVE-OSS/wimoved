#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H


#include "IPCQueue.h"
#include "EventHandler.h"
#include "IPCCaller.h"

class EventLoop : public EventHandler {
private:

    IPCQueue& queue;
    IPCCaller caller;
public:
    EventLoop(IPCQueue &queue, const std::string &iface);

    void loop();

    void handle_assoc(IPCAssocEvent* event) override;
    void handle_auth(IPCAuthEvent* event) override;
    void handle_disassoc(IPCDisassocEvent* event) override;
};


#endif //GAFFA_EVENTLOOP_H
