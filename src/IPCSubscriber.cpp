#include <stdexcept>
#include <iostream>
#include "IPCSubscriber.h"

IPCSubscriber::IPCSubscriber(IPCQueue &queue, const std::string& iface) : socket(iface), queue(queue) {

}

void IPCSubscriber::loop() {
    std::string result = socket.send_and_receive({"ATTACH"});
    if (result != "OK\n") {
        throw std::runtime_error(std::string("could not attach to hostapd: ") + result);
    }
    std::cout << "attached to hostapd" << std::endl;
    while (true) {
        std::string event = socket.receive();
        std::cout << event << std::endl;
    }
}
