#include <iostream>
#include "IPCSocket.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    IPCSocket sock(argv[1]);
    std::cout << "sending ping\n";
    std::string response = sock.send_command({"PING"});
    std::cout << "received " << response << "\n";
}