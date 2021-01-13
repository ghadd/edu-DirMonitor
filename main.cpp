#include "src/server.h"

int main(int argc, char *argv[]) {

   setupLoggers();

   server_logger = spdlog::get("server_logger");
   server_warn_logger = spdlog::get("server_warn_logger");

  if (argc < 2) {

      server_logger->warn("Not enough arguments");
      server_warn_logger->warn("Not enough arguments");
      exit(1);
  }

  server_logger->info("Success reading args from command line");
  server_logger->info("CmdLine: {} {}", argv[0], argv[1]);

  try {
    Server server(QString(argv[1]).toUInt());
    server.setupThis();

    return server.run();
  } catch (const std::runtime_error &error) {
    server_logger->error("{}", error.what());
    throw;
  }
}
