#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H


#include "IPCQueue.h"
#include "EventHandler.h"

class EventLoop : public EventHandler {
private:
    IPCQueue& queue;
public:
    explicit EventLoop(IPCQueue &queue);

    void loop();

    void handle_assoc(const IPCAssocEvent* event) override;
    void handle_auth(const IPCAuthEvent* event) override;
    void handle_disassoc(const IPCDisassocEvent* event) override;
};


#endif //GAFFA_EVENTLOOP_H
