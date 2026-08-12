#include <asm-generic/socket.h>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <poll.h>

int main() {

    // TCP SETUP
    int sockfd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    // sockfd is a file descriptor assigned by kernel as sockets are managed by os
    if(sockfd == -1) {
        std::cerr << "socket() failed\n";
        return 1;
    }

    // IPV4 Address setup
    sockaddr_in address{}; // built in sockaddr_in data type for ip addresses

    address.sin_family = AF_INET; // -> IPV4 address
    address.sin_port = htons(8080); // Convert host byte order short to network byte order

    if (inet_pton(
                AF_INET,
                "127.0.0.1",
                &address.sin_addr
            ) != 1) {

            std::cerr << "inet_pton() failed\n";
            close(sockfd);
            return 1;
        }

    // Connection to it
    // int result = connect( // this one is blocking I/O
        // sockfd,
        // reinterpret_cast<sockaddr*>(&address), // cause in connect here it is generic socket address
        // sizeof(address)
    // );

    int flags = fcntl(sockfd, F_GETFL, 0); // it is like configurations or settings towards the descriptor/resource

    if(flags == -1) {
        std::cerr << "fcntl(F_GETFL) failed: "
            << std::strerror(errno)
            << '\n';
        close(sockfd);
        return 1;
    }

    if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "fcntl(F_SETFL) failed: "
            << std::strerror(errno)
            << '\n';
        close(sockfd);
        return 1;
    }

    // Non blocking I/O
    int result = connect(
        sockfd,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    if(result == 0) {
        std::cout << "Connection successfull\n";
        close(sockfd);
        return 0;
    }

    if(errno == EINPROGRESS) {
        pollfd pfd{};
        pfd.fd = sockfd;
        pfd.events = POLLOUT;

        int poll_result = poll(
            &pfd,
            1,
            1000
        );

        if(poll_result == 0) {
            std::cout << "Connection timed out\n";
            close(sockfd);
            return 0;
        }

        if(poll_result == -1) {
            std::cerr << "poll() failed: "
                << std::strerror(errno)
                << '\n';
            close(sockfd);
            return 1;
        }

        std::cout << "Socket became ready\n";
        std::cout << "revents = " << pfd.revents << '\n';

        int socket_error = 0;
        socklen_t error_length = sizeof(socket_error);

        if(getsockopt(
            sockfd, // descriptor hogya socket ka
            SOL_SOCKET, // level hogya socket ka means kiss level mein organize kiya hain
            SO_ERROR, // means error do agar ho toh
            &socket_error, // ismei error daldo
            &error_length // error ki length ke sath
        ) == -1) {
            std::cerr << "getsockopt() failed: "
                << std::strerror(errno)
                << '\n';
            close(sockfd);
            return 1;
        }

        if(socket_error == 0) {
            std::cout << "Connection successfull\n";
        } else {
            std::cerr << "Connection failed: "
                << std::strerror(socket_error)
                << '\n';
        }

        close(sockfd);
        return 0;
    }

    // till this we can say like that oue non_blocking io can get events and managed timeout events
    //
    // we now have to manage the revents



    // actually linux use errno for failed operations the below one is quite generic for such
    // failures
    //
    // errno -> so it is a thread-local error indicator provided by the C/POSIX environment
    // why this cause it contains additional info about failure
    // well we have to convert to string so we can have human readable format of errno

    std::cerr << "connect() failed(): "
        << std::strerror(errno)
        << '\n';
    close(sockfd);
    return 1;
}
