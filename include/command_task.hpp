#pragma once
#include "tasks.hpp"
#include <string>

class CommandTask : public ITask {
public:
    explicit CommandTask(const std::string& cmd);
    void run(Logger& logger) override;
private:
    std::string command;
};
