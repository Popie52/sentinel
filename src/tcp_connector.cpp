#include "../include/tcp_connector.hpp"

#include <asm-generic/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <poll.h>

ConnectionResult TcpConnector::connect(
    const char * ip,
    int port,
    int timeout_ms) {
        int sockfd = socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

        if(sockfd == -1) {
            // std::cerr << "socket failed(): "
                // << std::strerror(errno) << '\n';
            // close(sockfd);
            return {ConnectionStatus::Error};
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        if(inet_pton(AF_INET,ip, &address.sin_addr) != 1) {
            close(sockfd);
            return {ConnectionStatus::Error};
        }

        int flags = fcntl(sockfd, F_GETFL, 0);

        if(flags == -1) {
            close(sockfd);
            return {ConnectionStatus::Error};
        }

        if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
            close(sockfd);
            return {ConnectionStatus::Error};
        }

        int result = ::connect(
            sockfd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        );

        if(result == 0) {
            close(sockfd);
            return {ConnectionStatus::Connected};
        }
        if(errno == EINPROGRESS) {

            pollfd pfd{};

            pfd.fd = sockfd;
            pfd.events = POLLOUT;

            int poll_result = poll(
                &pfd,
                1,
                timeout_ms
            );

            if(poll_result == 0) {
                close(sockfd);
                return {ConnectionStatus::Timeout};
            }

            if(poll_result == -1) {
                close(sockfd);
                return {ConnectionStatus::Error};
            }

            if(pfd.revents & POLLNVAL) {
                close(sockfd);
                return {ConnectionStatus::Error}; // invalid file descriptor
            }

            int socket_error = 0;
            socklen_t error_length = sizeof(socket_error);

            if(getsockopt(
                sockfd,
                SOL_SOCKET,
                SO_ERROR,
                &socket_error,
                &error_length
            ) == -1) {
                close(sockfd);
                return {ConnectionStatus::Error};
            }

            if(socket_error == 0) {
                close(sockfd);
                return {ConnectionStatus::Connected};
            }

            if(socket_error == ECONNREFUSED) {
                close(sockfd);
                return {ConnectionStatus::Refused};
            }

            close(sockfd);
            return {ConnectionStatus::Error};
        }
        if(errno == ECONNREFUSED) {
            close(sockfd);
            return {ConnectionStatus::Refused};
        }

        close(sockfd);
        return {ConnectionStatus::Error};
    }
