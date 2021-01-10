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
    // set mutex and port to listen by server
    explicit Server(int port) : port_(port) { pthread_mutex_init(&mutex, 0); }

    // set base setting of server(create socket, bind him and listen)
    void setupThis();

    // start server
    int run();

    ~Server();

private:
    /* private methods */
    // wrapper for handleClient to use in thread
    static void *handleClientWrapper(void *args);

    // get data that need client and send him
    void handleClient(client_t *client);

    // get client adrees
    void getClientAddress(struct sockaddr_in& addr);

    // get files info from specific directory to send them to client
    std::string generateResponse(QString buff);

    // handle errors and send response about the, to the client
    std::string handleError(std::string error);

    /* private variables */
    // socket descriptor
    int listenfd_{};

    // port to listen
    int port_;

    // server address
    sockaddr_in servAddr_{};
    socklen_t servAddrLen_{};

    // number of clients that are served by server
    unsigned int clientCount;

    // max connection of clients to server
    const unsigned int maxConnections = 10;

    pthread_mutex_t mutex;

    // save all descriptors of threads that are used by clients to delete in the end
    QVector<pthread_t*> handlesThread;

};

#endif