#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if(server_fd == -1) {
        std::cerr << "socket() failed\n";
        return 1;
    }

    std::cout << "Socket created: " << server_fd << '\n';

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &address.sin_addr
    );

    int result = bind(
        server_fd,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    if(result == -1) {
        std::cerr << "bind(): failed: " << std::strerror(errno) << '\n';
        close(server_fd);
        return 1;
    }

    std::cout << "socket bound successfully\n";

    // till above we just make os know that there is socket with local ip + port
    // thats it we didnt make it to listen yet

    if(listen(server_fd, 5) == -1) {
        std::cerr << "listen() failed: "
            << std::strerror(errno)
            << '\n';
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening\n";



    // till here program didnt wait for accepting the connection it just exits
    // so we need to manage it

    int client_fd = accept(
        server_fd,
        nullptr,
        nullptr
    ); // we are not asking kernel for clients information


    if (client_fd == -1) {
        std::cerr << "accept() failed: "
                  << std::strerror(errno)
                  << '\n';

        close(server_fd);
        return 1;
    }

    std::cout << "Client connected! FD = "
              << client_fd << '\n';


    close(client_fd);
    close(server_fd);

    return 0;
}
