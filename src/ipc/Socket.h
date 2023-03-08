#ifndef GAFFA_IPC_SOCKET_H
#define GAFFA_IPC_SOCKET_H

#include <sys/socket.h>
#include <sys/un.h>

#include <array>
#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace ipc {
const size_t MAC_ADDRESS_LENGTH = 2 * 6 + 5;

class Socket {
   public:
    explicit Socket(const std::chrono::duration<int> &timeout);

    std::string send_and_receive(const std::vector<std::string> &args);

    void send_command(const std::vector<std::string> &args);

    std::string receive();

    ~Socket();

   private:
    int sock_fd;
    struct sockaddr_un local;
    struct sockaddr_un dest;
};
}  // namespace ipc

#endif  // GAFFA_IPC_SOCKET_H
