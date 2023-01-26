#include <iostream>
#include "IPCSocket.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    IPCSocket sock(argv[1]);
    std::cout << "sending PING\n";
    std::cout << "received " << sock.send_and_receive({"PING"});
    std::cout << "sending ATTACH\n";
    std::cout << "received " << sock.send_and_receive({"ATTACH"});
    std::cout.flush();
    while (true) {
        std::cout << "received" << sock.receive() << "\n";
    }
}