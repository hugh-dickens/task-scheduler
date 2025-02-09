#include "logger.hpp"

#include <ctime>
#include <iostream>

static const std::string TASK_SCHEDULER_PATH = "task_scheduler.log";

Logger::Logger() {
    logFile.open(TASK_SCHEDULER_PATH, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file" << "\n";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

auto Logger::getInstance() -> Logger& {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        std::string timeStr = std::ctime(&now);

        // Remove newline character from time string
        timeStr.pop_back();

        std::cout << "[" << timeStr << "] " << message << "\n";
        logFile << "[" << timeStr << "] " << message << "\n";
        logFile.flush();
    } else {
        std::cerr << "Log file is not open" << "\n";
    }
}