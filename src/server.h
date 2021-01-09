#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// arman
class Server {
public:
    static const size_t maxConnections = 10;

    explicit Server(int port) : port_(port) {}

    void setupThis();

    static void *processRequest(void *requestData);
    int run();

private:
    int sockfd_{};
    int port_;
    sockaddr_in sockaddr_{};
    socklen_t sockaddrlen_{};
};

#endif
