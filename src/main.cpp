#include <iostream>
#include "../include/cli_parser.hpp"
#include "../include/tcp_connector.hpp"

int main(int argc, char** argv) {
    TcpConnector connector;
    ConnectionResult result = connector.connect(
        "127.0.0.1",
        8080,
        1000
    );

    switch(result.status) {
        case ConnectionStatus::Connected:
            std::cout << "CONNECTED\n";
            break;
        case ConnectionStatus::Refused:
            std::cout << "REFUSED\n";
            break;
        case ConnectionStatus::Timeout:
            std::cout << "TIMEOUT\n";
            break;
        case ConnectionStatus::Error:
            std::cout << "ERROR\n";
            break;
    }

    // auto config = parse_arguments(argc, argv);

    // if(!config) {
    //     return 1;
    // }

    // std::cout << "Target: " << config->target << '\n';

    return 0;
}
