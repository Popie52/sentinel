#pragma once

enum class ConnectionStatus{
    Connected,
    Refused,
    Timeout,
    Error,
};

struct ConnectionResult {
    ConnectionStatus status;
};

class TcpConnector {
    public:
        ConnectionResult connect(
            const char * ip,
            int port,
            int timeout_ms
        );
};
