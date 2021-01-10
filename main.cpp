#include "src/server.h"

int main() {
    Server server(9992);
    server.setupThis();

    return server.run();
}
