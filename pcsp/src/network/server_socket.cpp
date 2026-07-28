#include "pcsp/network.hpp"
#include "pcsp/protocol.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int start_server(uint16_t port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return -1;

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        close(server_fd);
        return -1;
    }
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0){
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0){
        close(server_fd);
        return -1;
    }
    return server_fd;
}

void run_iterative_loop(int server_fd){
    while (true){
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_len);
        if (client_fd < 0){
            continue;
        }
        parse_incoming_stream(client_fd);
        close(client_fd);
    } 
}
