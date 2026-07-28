#include "pcsp/network.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int connect_to_server(const char* ip_address, uint16_t port) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0)
        return -1;

    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    //  IP address string to binary
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        close(sock_fd);
        return -1;
    }

    // Connect to server
    if (connect(sock_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}