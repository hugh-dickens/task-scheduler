#pragma once

#include "logger.hpp"
#include <chrono>
#include <memory>
#include <string>

// Represents different types of tasks.
enum class TaskType { COMMAND, FILE_PROCESS };

// Interface for tasks.
class ITask {
public:
    virtual ~ITask() = default;
    virtual void run(Logger& logger) = 0;
};

// Struct to hold a task and its scheduled execution time.
struct ScheduledTask {
    std::chrono::time_point<std::chrono::system_clock> executeAt;
    std::unique_ptr<ITask> task;

    // Priority queue orders tasks so that the earliest executeAt is on top.
    bool operator<(const ScheduledTask& other) const {
        return executeAt > other.executeAt;
    }
};
