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

    virtual void handle_assoc(const IPCEvent* event) override;
    virtual void handle_auth(const IPCEvent* event) override;
    virtual void handle_disassoc(const IPCEvent* event) override;
};


#endif //GAFFA_EVENTLOOP_H
