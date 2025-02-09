#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ITaskScheduler {
   public:
    virtual void scheduleTask(const std::string& command, int delay) = 0;
    virtual void run() = 0;
    virtual ~ITaskScheduler() = default;
};

struct Task {
    std::string command;
    std::chrono::time_point<std::chrono::system_clock> executeAt;

    bool operator<(const Task& other) const { return executeAt > other.executeAt; }
};

class TaskScheduler : public ITaskScheduler {
   public:
    void scheduleTask(const std::string& command, int delay) override;
    void run() override;

   private:
    std::priority_queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable taskCondition;
    bool running = true;

    void processTasks();
};