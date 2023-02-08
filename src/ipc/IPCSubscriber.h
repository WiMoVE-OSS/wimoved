#ifndef GAFFA_IPCSUBSCRIBER_H
#define GAFFA_IPCSUBSCRIBER_H

#include "IPCSocket.h"
#include "IPCQueue.h"

class IPCSubscriber {
public:
    explicit IPCSubscriber(IPCQueue& queue, const std::string& iface);
    void loop();
private:
    IPCSocket socket;
    IPCQueue& queue;
};

#endif // GAFFA_IPCSUBSCRIBER_H
