#include "src/server.h"

int main() {
    Server server(9999);
    server.setupThis();

    return server.run();
}
