#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "dirmonitor.h"

#include <QVector>
#include <QRegExp>
#include <QStringList>
#include <QPair>

#define BUFFER_SZ (1 << 10)

struct client_t{
    struct sockaddr_in address;
    int sockfd;
};


class Server {
public:

    explicit Server(int port) : port_(port) { pthread_mutex_init(&mutex, 0); }
    void setupThis();

    static void *handleClientWrapper(void *args);
    void handleClient(client_t *client);
    void getClientAddress(struct sockaddr_in& addr);
    int run();

protected:
    std::string generateResponse(QString buff);

private:
    int listenfd_{};

    int port_;

    sockaddr_in servAddr_{};
    socklen_t servAddrLen_{};

    unsigned int clientCount;
    const unsigned int maxConnections = 10;

    pthread_mutex_t mutex;
};

#endif
