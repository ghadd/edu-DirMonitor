#include "src/server.h"

int main() {
    try {
        Server server(9998);
        server.setupThis();

        return server.run();
    }  catch (const std::runtime_error& error) {
        /*
         * TODO EXCEPTION
         */
        throw;
    }
}
