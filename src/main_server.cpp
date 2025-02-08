#include "server.hpp"
#include "scheduler.hpp"

int main() {
    TaskScheduler scheduler;
    scheduler.run(); // Start the scheduler in the background

    TaskServer server(8080);
    server.start(); // Start the server to listen for client connections

    return 0;
}

