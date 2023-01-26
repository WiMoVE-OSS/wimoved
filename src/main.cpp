#include <iostream>
#include "IPCSocket.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <interface>\n";
        return 1;
    }
    IPCSocket sock(argv[1]);
    sock.send_command({"PING"});
}