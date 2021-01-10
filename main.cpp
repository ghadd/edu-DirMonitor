#include "src/server.h"

int main() {
    Server server(9994);
    server.setupThis();

    return server.run();
}
