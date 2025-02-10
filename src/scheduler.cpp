#include "scheduler.hpp"

#include <cstdlib>

#include "logger.hpp"

void TaskScheduler::scheduleTask(const std::string& command, int delay) {
    std::lock_guard<std::mutex> lock(queueMutex);

    Task task;
    task.command = command;
    task.executeAt = std::chrono::system_clock::now() + std::chrono::seconds(delay);
    taskQueue.push(task);

    taskCondition.notify_one();
}

void TaskScheduler::run() { std::thread(&TaskScheduler::processTasks, this).detach(); }

void TaskScheduler::processTasks() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);

        taskCondition.wait(lock, [this] { return !taskQueue.empty() || !running; });

        while (!taskQueue.empty()) {
            auto now = std::chrono::system_clock::now();
            Task task = taskQueue.top();

            if (task.executeAt > now) {
                taskCondition.wait_until(lock, task.executeAt);
                continue;
            }

            taskQueue.pop();
            lock.unlock();

            int result = std::system(task.command.c_str());
            if (result != 0) {
                std::cerr << "Command failed: " << task.command << "\n";
                Logger::getInstance().log("Task failed: " + task.command);
            } else {
                Logger::getInstance().log("Task completed: " + task.command);
            }

            lock.lock();
        }
    }
}