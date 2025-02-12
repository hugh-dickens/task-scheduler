#include "task.hpp"

// overload operator<< for TaskType to help with debugging
std::ostream& operator<<(std::ostream& os, TaskType type) {
    switch (type) {
        case TaskType::COMMAND:
            os << "COMMAND";
            break;
        case TaskType::FILE_PROCESS:
            os << "FILE_PROCESS";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}