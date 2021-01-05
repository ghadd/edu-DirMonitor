#include "server.h"

void Server::setupThis() {

// Create a socket (IPv4, TCP)
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ == -1) {
        throw std::runtime_error("Failed to create socket. errno: ");
    }

// Listen to port on any address
    sockaddr_.sin_family = AF_INET;
    sockaddr_.sin_addr.s_addr = INADDR_ANY;
    sockaddr_.sin_port = htons(port_); // htons is necessary to convert a number
// to network byte order

    if (bind(sockfd_, (sockaddr *) &sockaddr_, sizeof(sockaddr)) < 0) {
        throw std::runtime_error("Failed to bind to port {}. errno: {}");
    }

    if (listen(sockfd_, maxConnections) < 0) {
        throw std::runtime_error("Failed to listen on socket. errno: {}");
    }
}

void *Server::processRequest(void *requestData) {
    /* TODO
     * 1. Parse request data
     * 2. Call DirMonitor
     * 3. Dump a response
     * 4. Return a response data
     */
    free(requestData);

    return nullptr;
}

int Server::run() {
    /* TODO:
     * 1. Implement the whole transaction functionality
     * 2. Implement logging
     */
    sockaddrlen_ = sizeof(sockaddr_);
    int conn;
    while ((conn = accept(sockfd_, (struct sockaddr *) &sockaddr_, &sockaddrlen_)) > 0) {
        // This region is gotta be replaced
        char buffer[100];
        read(conn, buffer, 100);
        std::cout << "The message was: " << buffer << std::endl;

        std::string response = "Good talking to you\n";
        send(conn, response.c_str(), response.size(), 0);

        close(conn);
        // end of region
    }

    close(sockfd_);

    return 0;
};
