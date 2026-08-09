#include <iostream>
#include "../include/cli_parser.hpp"

int main(int argc, char** argv) {
    auto config = parse_arguments(argc, argv);

    if(!config) {
        return 1;
    }

    std::cout << "Target: " << config->target << '\n';

    return 0;
}
