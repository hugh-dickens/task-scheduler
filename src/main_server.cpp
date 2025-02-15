#include "scheduler.hpp"
#include "security.hpp"
#include "task_server.hpp"
#include "logger.hpp"

auto main() -> int {
    preventRootExecution();

    Logger& logger = Logger::getInstance(); // Singleton logger instance

    TaskScheduler scheduler(logger);
    scheduler.run();  // start the scheduler in the background

    TaskServer server(8080, scheduler);
    server.start();  // start the server to listen for client connections

    scheduler.stop();  // stop the scheduler when the server is done
    return 0;
}
