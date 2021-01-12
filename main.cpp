#include "src/server.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
      std::cout << "Not enough arguments.";
      exit(1);
  }
  try {
    Server server(QString(argv[1]).toUInt());
    server.setupThis();

    return server.run();
  } catch (const std::runtime_error &error) {
    /*
     * TODO EXCEPTION
     */
    throw;
  }
}
