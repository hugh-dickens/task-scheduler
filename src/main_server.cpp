#include "server.hpp"
#include "scheduler.hpp"

int main() {
    TaskScheduler scheduler;
    scheduler.run(); // start the scheduler in the background

    TaskServer server(8080, scheduler);
    server.start(); // start the server to listen for client connections

    return 0;
}

