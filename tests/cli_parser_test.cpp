#include <cassert>
#include <vector>

#include "../include/cli_parser.hpp"

bool parse_target(const std::string& target) {
    char arg0[] = "sentinel";
    char arg1[] = "scan";

    std::vector<char> target_buffer(target.begin(), target.end());
        target_buffer.push_back('\0');

    char* argv[] = {
        arg0,
        arg1,
        target_buffer.data()
    };

    auto result = parse_arguments(3, argv);

    return result.has_value();
}


void test_invalid_ipv4() {
    assert(!parse_target("256.0.0.1"));
    assert(!parse_target("127.0.0.999"));
    assert(!parse_target("127.0.0"));
    assert(!parse_target("127..0.1"));
    assert(!parse_target("127.0.0."));
    assert(!parse_target("127.a.0.1"));
}

void test_missing_arguments() {
    char arg0[] = "sentinel";

    char* argv[] = {
        arg0
    };

    auto result = parse_arguments(1, argv);

    assert(!result.has_value());
}

void test_missing_target() {
    char arg0[] = "sentinel";
    char arg1[] = "scan";

    char* argv[] = {
        arg0,
        arg1
    };

    auto result = parse_arguments(2, argv);

    assert(!result.has_value());
}

void test_unknown_command() {
    char arg0[] = "sentinel";
    char arg1[] = "hello";
    char arg2[] = "127.0.0.1";

    char* argv[] = {
        arg0,
        arg1,
        arg2
    };

    auto result = parse_arguments(3, argv);

    assert(!result.has_value());
}

int main() {

    test_invalid_ipv4();

    test_missing_arguments();

    test_missing_target();

    test_unknown_command();

    return 0;
}
