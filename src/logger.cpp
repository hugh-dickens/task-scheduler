#include "logger.hpp"
#include <iostream>
#include <ctime>

static const std::string TASK_SCHEDULER_PATH = "task_scheduler.log";

Logger::Logger() {
    logFile.open(TASK_SCHEDULER_PATH, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file" << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
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

        std::cout << "[" << timeStr << "] " << message << std::endl;
        logFile << "[" << timeStr << "] " << message << "\n";
        logFile.flush();
    } else {
        std::cerr << "Log file is not open" << std::endl;
    }
}