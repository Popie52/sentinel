cmake_minimum_required(VERSION 3.20)

project(sentinel
    VERSION 0.1.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sentinel
    src/main.cpp
    src/cli_parser.cpp
)

enable_testing()

add_executable(cli_parser_test
    tests/cli_parser_test.cpp
    src/cli_parser.cpp
)

target_include_directories(cli_parser_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_test(
    NAME cli_parser_test
    COMMAND cli_parser_test
)
