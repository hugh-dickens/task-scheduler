#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "task.hpp"

class ITaskScheduler {
   public:
    virtual void scheduleTask(const std::string& input, int delay, TaskType type) = 0;
    virtual void run() = 0;
    virtual ~ITaskScheduler() = default;
};

class TaskScheduler : public ITaskScheduler {
   public:
    void scheduleTask(const std::string& input, int delay, TaskType type) override;
    void run() override;

   private:
    std::priority_queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable taskCondition;
    bool running = true;

    void processFile(const std::string& filePath);
    void convertJsonToCsv(const std::string& jsonFilePath, const std::string& csvFilePath);
    void convertCsvToJson(const std::string& csvFilePath, const std::string& jsonFilePath);
    void processTasks();
};