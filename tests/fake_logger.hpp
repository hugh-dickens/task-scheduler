#include <gtest/gtest.h>
#include "scheduler.hpp"
#include <vector>
#include <string>

// FakeLogger: a simple logger that records log messages.
class FakeLogger : public Logger {
public:
    std::vector<std::string> logs;
    virtual void log(const std::string& message) override {
        logs.push_back(message);
    }
};