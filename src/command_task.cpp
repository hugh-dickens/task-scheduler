#include "command_task.hpp"
#include <cstdlib>
#include <iostream>

CommandTask::CommandTask(const std::string& cmd)
    : command(cmd) {}

void CommandTask::run(Logger& logger) {
    std::cout << "[INFO] Executing command: " << command << "\n";
    int result = std::system(command.c_str());
    if (result != 0) {
        logger.log("Task failed: " + command);
    } else {
        logger.log("Task completed: " + command);
    }
}
