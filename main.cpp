#include "src/server.h"

int main() {
    Server server(9993);
    server.setupThis();

    return server.run();
}
