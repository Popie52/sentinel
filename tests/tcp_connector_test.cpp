#include "../include/tcp_connector.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

    TcpConnector connector;

    // Test 1: Connection refused
    auto refused = connector.connect(
        "127.0.0.1",
        9999,
        1000
    );

    if (refused.status != ConnectionStatus::Refused) {
        return 1;
    }

    // Test 2: Connection timeout
    auto timeout = connector.connect(
        "192.0.2.1",
        8080,
        100
    );

    if (timeout.status != ConnectionStatus::Timeout) {
        return 1;
    }

    // Test 3: Successful connection
    int server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (server_fd == -1) {
        return 1;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(8081);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(
            server_fd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) == -1) {

        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 1) == -1) {
        close(server_fd);
        return 1;
    }

    auto connected = connector.connect(
        "127.0.0.1",
        8081,
        1000
    );

    close(server_fd);

    if (connected.status != ConnectionStatus::Connected) {
        return 1;
    }

    return 0;
}
