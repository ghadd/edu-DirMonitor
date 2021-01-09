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

#define BUFFER_SZ 2048

class Server {
public:

    explicit Server(int port) : port_(port) { pthread_mutex_init(&mutex, 0); }
    void setupThis();
    void *handleClient(void *client);
    void getClientAddress(struct sockaddr_in& addr);
    int run();

private:
    /* Client structure */
    typedef struct{
        struct sockaddr_in address;
        int sockfd;
    } client_t;


    int listenfd_{};

    int port_;

    sockaddr_in servAddr_{};
    socklen_t servAddrLen_{};

    unsigned int clientCount;
    const unsigned int maxConnections = 10;

    pthread_mutex_t mutex;
};

#endif
