#include <chrono>
#include <iostream>

enum class TaskType { COMMAND, FILE_PROCESS };

// Declare operator<< for TaskType
std::ostream& operator<<(std::ostream& os, TaskType type);

struct Task {
    TaskType type;
    std::string command;
    std::string filePath;
    std::chrono::time_point<std::chrono::system_clock> executeAt;

    bool operator<(const Task& other) const { return executeAt > other.executeAt; }
};
