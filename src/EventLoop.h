#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H


#include "ipc/IPCQueue.h"
#include "EventHandler.h"
#include "ipc/IPCCaller.h"
#include "NetworkRenderer.h"

class EventLoop : public EventHandler {
private:
    NetworkRenderer& renderer;
    IPCQueue& queue;
    IPCCaller caller;
public:
    EventLoop(NetworkRenderer& renderer, IPCQueue &queue, const std::string &iface);

    void loop();

    void handle_assoc(IPCAssocEvent* event) override;
    void handle_auth(IPCAuthEvent* event) override;
    void handle_disassoc(IPCDisassocEvent* event) override;
};


#endif //GAFFA_EVENTLOOP_H
