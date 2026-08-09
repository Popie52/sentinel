#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <cctype>
#include "../include/scan_config.hpp"
#include "../include/cli_parser.hpp"


bool is_valid_ipv4_octet(const std::string& octet) {
    if(octet.empty()) {
        return false;
    }

    for(char c: octet) {
        if(!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    try {
        int value = std::stoi(octet);
        return value >= 0 && value <= 255;
    } catch (const std::invalid_argument&) {
        return false;
    } catch(const std::out_of_range&) {
        return false;
    }
}

std::optional<ScanConfig> parse_arguments(
    int argc,
    char* argv[]
) {
    if(argc < 3) {
        std::cerr << "Error: missing arguments\n";
        std::cerr << "Usage: sentinel scan <target>\n";
        return std::nullopt;
    }

    std::string command = argv[1];

    if(command != "scan") {
        std::cerr << "Error: unknown command: " << command << '\n';
        std::cerr << "Usage: sentinel scan <target>\n";
        return std::nullopt;
    }

    std::string target = argv[2];

    std::string temp;
    int dot_count = 0;
    for(char c: target) {
        if(c == '.') {
            if(!is_valid_ipv4_octet(temp)) {
                std::cerr << "Error: Invalid Target IP\n";
                return std::nullopt;
            }
            ++dot_count;
            temp.clear();
        } else {
            if(!std::isdigit(static_cast<unsigned char>(c))) {
                std::cerr << "Error: Invalid Target IP\n";
                return std::nullopt;
            }

            temp += c;
        }
    }

    if(!is_valid_ipv4_octet(temp)) {
        std::cerr << "Error: Invalid Target IP\n";
        return std::nullopt;
    }

    if(dot_count != 3) {
        std::cerr << "Error: Invalid Target IP\n";
        return std::nullopt;
    }

    ScanConfig config;
    config.target = target;

    return config;
}
