#ifndef WIMOVED_IPC_SOCKET_H
#define WIMOVED_IPC_SOCKET_H

#include <sys/socket.h>
#include <sys/un.h>

#include <array>
#include <chrono>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace ipc {
const size_t MAC_ADDRESS_LENGTH = 2 * 6 + 5;

class Socket {
   public:
    explicit Socket(const std::chrono::duration<int> &timeout, const std::string &sockname);
    ~Socket();
    Socket(const Socket &other) = delete;
    void operator=(const Socket &other) = delete;
    Socket(Socket &&other) noexcept;

    std::string send_and_receive(const std::vector<std::string> &args);
    void send_command(const std::vector<std::string> &args);
    std::string receive();

    inline static const std::string HOSTAPD_OK = "OK\n";

   private:
    int sock_fd;
    struct sockaddr_un local;
    struct sockaddr_un dest;
};
}  // namespace ipc

#endif  // WIMOVED_IPC_SOCKET_H
