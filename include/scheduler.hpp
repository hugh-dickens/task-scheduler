#pragma once

#include "tasks.hpp"
#include "file_conversions/file_handler.hpp"
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

// Interface for scheduling tasks.
class ITaskScheduler {
public:
    virtual void scheduleTask(std::unique_ptr<ITask> task, int delay) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual ~ITaskScheduler() = default;
};

// TaskScheduler is responsible for scheduling and dispatching tasks.
class TaskScheduler : public ITaskScheduler {
public:
    // Constructor receives a Logger reference via dependency injection.
    explicit TaskScheduler(Logger& logger);
    ~TaskScheduler();

    // Schedules a task to run after 'delay' seconds.
    void scheduleTask(std::unique_ptr<ITask> task, int delay) override;
    // Starts the background worker thread.
    void run() override;
    // Signals the worker to stop and waits for it to finish.
    void stop() override;

private:
    void processTasks();
    void cleanup();

    Logger& logger;
    bool running;
    std::vector<ScheduledTask> taskHeap;
    std::mutex queueMutex;
    std::condition_variable taskCondition;
    std::thread workerThread;

    std::unordered_map<std::string, std::unique_ptr<IFileHandler>> fileHandlers;
};
