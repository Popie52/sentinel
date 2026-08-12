# Sentinel

A lightweight TCP network scanning tool written in modern C++20 for Linux.

Sentinel is being developed from the ground up to explore low-level networking, POSIX socket programming, non-blocking I/O, connection timeouts, error handling, CLI design, and automated testing.

> **Status:** Early development — TCP connectivity and CLI foundations are implemented; full scanning functionality is still in progress.

## Features

### TCP Connectivity

Sentinel currently implements a TCP connector using Linux/POSIX networking APIs.

- IPv4 TCP connections
- Non-blocking sockets
- Connection timeout handling
- `poll()`-based I/O waiting
- Connection result detection using `SO_ERROR`
- Explicit connection status handling:
  - `CONNECTED`
  - `REFUSED`
  - `TIMEOUT`
  - `ERROR`
- Automatic socket cleanup

### Command-Line Parsing

A command-line parser is implemented for the planned scanning interface:

```text
sentinel scan <target>

The parser currently validates:

scan command
IPv4 addresses
IPv4 octets in the range 0-255
Missing arguments
Unknown commands
Malformed IPv4 addresses
Automated Testing

Sentinel uses CTest for automated testing.

Current test coverage includes:

IPv4 validation
Missing command-line arguments
Missing target
Unknown commands
Refused TCP connections
TCP connection timeouts
Successful TCP connections
Architecture

The project is currently organized into small, focused components:

sentinel/
├── include/
│   ├── cli_parser.hpp
│   ├── scan_config.hpp
│   └── tcp_connector.hpp
│
├── src/
│   ├── cli_parser.cpp
│   ├── main.cpp
│   └── tcp_connector.cpp
│
├── tests/
│   ├── cli_parser_test.cpp
│   └── tcp_connector_test.cpp
│
├── experiments/
│   ├── tcp_client.cpp
│   └── tcp_server.cpp
│
├── CMakeLists.txt
└── README.md
Components

CLI Parser

Responsible for validating command-line arguments and IPv4 targets.

TCP Connector

Encapsulates TCP connection logic and returns a structured connection result:

enum class ConnectionStatus {
    Connected,
    Refused,
    Timeout,
    Error
};

Tests

Independent executables are built for CLI parsing and TCP connectivity and registered with CTest.

Building
Requirements
Linux
C++20 compiler
CMake 3.20+
POSIX socket support
Build
git clone https://github.com/Popie52/sentinel.git
cd sentinel

cmake -S . -B build
cmake --build build
Running Tests

Run the complete test suite with:

ctest --test-dir build --output-on-failure

Example:

Test project /path/to/sentinel/build
    Start 1: cli_parser_test
1/2 Test #1: cli_parser_test .......... Passed
    Start 2: tcp_connector_test
2/2 Test #2: tcp_connector_test ........ Passed
Current Development

The project is being developed incrementally.

Completed
 C++20 project structure
 CMake build system
 IPv4 command-line validation
 TCP socket creation
 Non-blocking TCP connections
 Connection timeout handling
 poll()-based connection waiting
 Connection error detection with SO_ERROR
 Connection status abstraction
 CLI parser tests
 TCP connector tests
 CTest integration
In Progress
 Connect CLI arguments to the scanning engine
 Port scanning
 Port-range configuration
 Scan result reporting
 Improved command-line interface
 Additional networking tests
 Performance measurements
 Further modularization of the scanning engine
Design Goals

Sentinel is being built with a focus on understanding the fundamentals rather than relying on high-level networking libraries.

The project aims to provide practical experience with:

C++ systems programming
POSIX sockets
TCP/IP networking
Non-blocking I/O
File descriptors
poll()
Connection state handling
Error handling
Resource management
CMake
Automated testing
Linux networking
Why Sentinel?

The goal is to build a networking tool incrementally while understanding how the underlying operating-system and networking primitives work.

Instead of starting with a high-level scanning library, Sentinel implements the networking layer directly using POSIX socket APIs and builds higher-level scanning functionality on top of it.
