#include "server.h"

// set base setting of server(create socket, bind him and listen)
void Server::setupThis() {

  // Create a socket (IPv4, TCP)
  listenfd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (listenfd_ == -1) {
    throw std::runtime_error("Failed to create socket. errno: ");
  }

  // Listen to port on any address
  servAddr_.sin_family = AF_INET; // set IPv4
  servAddr_.sin_addr.s_addr = INADDR_ANY;
  servAddr_.sin_port = htons(port_); // htons is necessary to convert a number

  // bind socket to specific address
  if (bind(listenfd_, (sockaddr *)&servAddr_, sizeof(sockaddr)) < 0) {
    throw std::runtime_error("Failed to bind to port {}. errno: {}");
  }

  if (listen(listenfd_, maxConnections) < 0) {
    throw std::runtime_error("Failed to listen on socket. errno: {}");
  }
}

// wrapper for handleClient to use in thread
void *Server::handleClientWrapper(void *args) {
  auto data = *static_cast<QPair<Server *, client_t *> *>(args);
  data.first->handleClient(data.second);

  return nullptr;
}

// handle errors and send response about the, to the client
std::string Server::handleError(std::string error) {
  json responseError = {{"status", false}, {"reason", error}};
  return responseError.dump();
}

// get files info from specific directory to send them to client
std::string Server::generateResponse(QString buff) {
  std::string response;

  // get list, where first argument is path ; second is extentions of files
  QStringList pathAndFormats = buff.split(QRegExp("\n"));

  // get path to directory
  QString path = pathAndFormats[0].trimmed();

  /* TOLOG
  std::cout << "\nPath: " << path.toStdString() << std::endl;
  */

  QStringList formats = QStringList();
  // check if client enter formats
  if (pathAndFormats.size() > 1) {
    QString formatsString = pathAndFormats[1];
    formats = formatsString.split(QRegExp(" | "));
    if (formats.at(0).isEmpty()) {
      formats = QStringList();
    }
  }

  /* TOLOG
  std::cout << "\nFormats: ";
  for(auto &format : formats)
      std::cout << format.trimmed().toStdString() << ", " << std::endl;
  */

  // get info about files of formats(second argument) from path or
  // directory(first argument)
  DirMonitor monitor(path, formats);

  // check if path and formats of files are valid
  try {
    monitor.validatePath();
  } catch (const PathError &e) {
    response = handleError(
        "You didn't enter valid path, change it please."); // responseError.dump();
    return response;
  }

  // if NO ERROR -> get files info from specific directory to send them to
  // client
  auto responseData = monitor.applyMonitor();
  if (responseData.first.size() == 0) {
    response = handleError("No files mathing given extensions found.");
    return response;
  }

  auto responseJson = DirMonitor::jsonify(responseData);
  responseJson["status"] = true;
  response = responseJson.dump();
  return response;
}

// handle client request
void Server::handleClient(client_t *client) {

  /* TOLOG
  std::cout << "Connected: ";
  getClientAddress(client->address);
  */

  char *buffer = new char[BUFFER_SZ];

  while (client) {
    // read path to directory and extentions of files that are in this directory
    read(client->sockfd, buffer, BUFFER_SZ);

//    /* TOLOG
    std::cout << "\nThe message was: " << buffer << std::endl;
//    */

    QString buff(buffer);

    // if message from client starts with word '__close__' - close connection
    if (buff.startsWith("__close__")) {
      close(client->sockfd);
      return;
    } else if (buff.startsWith("__test__")) {
        std::string response = ((json){{"status", true}}.dump());
        send(client->sockfd, response.c_str(), response.size(), 0);
    } else {
        // get files info from specific directory to send them to client
        auto response = generateResponse(buff);

        // send response
        send(client->sockfd, response.c_str(), response.size(), 0);
    }
  }

  delete[] buffer;
  delete client;

  // decrease number of client that are served by server
  pthread_mutex_lock(&mutex);
  clientCount--;
  pthread_mutex_unlock(&mutex);
}

// get IP of client
void Server::getClientAddress(struct sockaddr_in &addr) {
  printf("%d.%d.%d.%d", addr.sin_addr.s_addr & 0xff,
         (addr.sin_addr.s_addr >> 8) & 0xff,
         (addr.sin_addr.s_addr >> 16) & 0xff,
         (addr.sin_addr.s_addr >> 24) & 0xff);
}

// run server
int Server::run() {
  /* TODO:
   * 1. Implement the whole transaction functionality
   * 2. Implement logging
   */

  // client socket
  int connfd;

  // client socket addr
  sockaddr_in clientAddr{};
  socklen_t clientAddrLen = sizeof(clientAddr);

  printf("=== SERVER STARTED WORKING ===\n");

  while ((connfd = accept(listenfd_, (struct sockaddr *)&clientAddr,
                          &clientAddrLen)) > 0) {
    if ((clientCount + 1) == maxConnections) {
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
    cli->address = clientAddr; // set address if client
    cli->sockfd = connfd;      // set socket number of client

    // descriptor of thread
    pthread_t threadHandle;

    auto data = qMakePair(this, cli);
    // handle each client
    pthread_create(&threadHandle, NULL, &handleClientWrapper,
                   static_cast<void *>(&data));

    handlesThread.push_back(&threadHandle);
  }

  close(listenfd_);

  return 0;
}

Server::~Server() {
  for (int i = 0; i < handlesThread.size(); i++)
    delete handlesThread[i];
  close(listenfd_);
};
