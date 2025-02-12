#include "scheduler.hpp"
#include "security.hpp"
#include "task_server.hpp"

auto main() -> int {
    preventRootExecution();

    TaskScheduler scheduler;
    scheduler.run();  // start the scheduler in the background

    TaskServer server(8080, scheduler);
    server.start();  // start the server to listen for client connections

    return 0;
}
