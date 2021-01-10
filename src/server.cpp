#include "server.h"

void Server::setupThis() {

    // Create a socket (IPv4, TCP)
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ == -1) {
        throw std::runtime_error("Failed to create socket. errno: ");
    }

    // Listen to port on any address
    servAddr_.sin_family = AF_INET;
    servAddr_.sin_addr.s_addr = INADDR_ANY;
    servAddr_.sin_port = htons(port_); // htons is necessary to convert a number

    // to network byte order
    if (bind(listenfd_, (sockaddr *) &servAddr_, sizeof(sockaddr)) < 0) {
        throw std::runtime_error("Failed to bind to port {}. errno: {}");
    }

    if (listen(listenfd_, maxConnections) < 0) {
        throw std::runtime_error("Failed to listen on socket. errno: {}");
    }
}

void *Server::handleClientWrapper(void *args)
{
    auto data = *static_cast<QPair<Server *, client_t *> *>(args);
    data.first->handleClient(data.second);

    return nullptr;
}

std::string Server::generateResponse(QString buff)
{
    QStringList pathAndFormats = buff.split(QRegExp("\n"));

    // get path to directory
    QString path = pathAndFormats[0].trimmed();
    std::cout << "\nPath: " << path.toStdString() << std::endl;

    // get extentions of files in directory
    QString formatsString = pathAndFormats[1];
    QStringList formats = formatsString.split(QRegExp(" | "));
    if (formatsString.isEmpty()) formats = QStringList();
    std::cout << "\nFormats: ";

    for(auto &format : formats)
        std::cout << format.trimmed().toStdString() << ", " << std::endl;

    DirMonitor monitor(path, formats);
    monitor.validatePath();

    std::string response = DirMonitor::jsonify(monitor.applyMonitor()).dump();

    return response;
}

void Server::handleClient(client_t *client) {

    std::cout << "Connected: ";

    /* TOLOG
    getClientAddress(client->address);
    */

    char *buffer = new char[BUFFER_SZ];
    while (client) {
        read(client->sockfd, buffer, BUFFER_SZ);
        std::cout << "\nThe message was: " << buffer << std::endl;
        QString buff(buffer);

        if (buff.startsWith("__close__")) {
            close(client->sockfd);
            return;
        }

        auto response = generateResponse(buff);

        send(client->sockfd, response.c_str(), response.size(), 0);
    }
    delete []buffer;
    delete client;

    pthread_mutex_lock(&mutex);
    clientCount--;
    pthread_mutex_unlock(&mutex);
}

void Server::getClientAddress(struct sockaddr_in& addr) {
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr >> 8) & 0xff,
        (addr.sin_addr.s_addr >> 16) & 0xff,
        (addr.sin_addr.s_addr >> 24) & 0xff);
}

int Server::run() {
    /* TODO:
     * 1. Implement the whole transaction functionality
     * 2. Implement logging
     */

    // client socket
    int connfd;

    // client socket addr
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof (clientAddr);

    printf("=== SERVER START WORKING ===\n");

    // keep all handles of threads
    QVector<pthread_t*> handlesThread;


    while ((connfd = accept(listenfd_, (struct sockaddr *) &clientAddr, &clientAddrLen)) > 0) {
        if((clientCount + 1) == maxConnections){
            printf("Max clients reached. Rejected: ");
            getClientAddress(clientAddr);
            printf(":%d\n", clientAddr.sin_port);
            close(connfd);
            continue;
        }

        // increase number of clients that are served by server
        clientCount++;

        // client settings
        client_t *cli = new client_t;
        cli->address = clientAddr;
        cli->sockfd = connfd;

        // descriptor of thread
        pthread_t threadHandle;

        auto data = qMakePair(this, cli);
        // handle each client
        pthread_create(&threadHandle, NULL, &handleClientWrapper, static_cast<void *>(&data));

        handlesThread.push_back(&threadHandle);
    }

    for(int i = 0; i < handlesThread.size(); i++)
        delete handlesThread[i];

    close(listenfd_);

    return 0;
};
