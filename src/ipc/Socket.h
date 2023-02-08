#ifndef GAFFA_IPC_SOCKET_H
#define GAFFA_IPC_SOCKET_H

#include <array>
#include <optional>
#include <string>
#include <vector>

#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {
    const size_t MAC_ADDRESS_LENGTH = 2 * 6 + 5;

    class Socket {
    public:
        explicit Socket(const std::string &iface, const std::chrono::duration<int> &timeout);

        std::string send_and_receive(const std::vector<std::string> &args) const;

        void send_command(const std::vector<std::string> &args) const;

        std::string receive() const;

        ~Socket();

    private:
        int sock_fd;
        struct sockaddr_un local;
        struct sockaddr_un dest;
    };
}

#endif // GAFFA_IPC_SOCKET_H
