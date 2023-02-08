#ifndef GAFFA_EVENTLOOP_H
#define GAFFA_EVENTLOOP_H


#include "ipc/Queue.h"
#include "EventHandler.h"
#include "ipc/Caller.h"
#include "NetworkRenderer.h"

class EventLoop : public EventHandler {
private:
    NetworkRenderer& renderer;
    ipc::Queue& queue;
    ipc::Caller caller;
public:
    EventLoop(NetworkRenderer& renderer, ipc::Queue &queue, const std::string &iface);

    void loop();

    void handle_assoc(ipc::AssocEvent* event) override;
    void handle_auth(ipc::AuthEvent* event) override;
    void handle_disassoc(ipc::DisassocEvent* event) override;
};


#endif //GAFFA_EVENTLOOP_H
