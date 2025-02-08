#include <cstdlib>
#include "scheduler.hpp"

void TaskScheduler::scheduleTask(const std::string& command, int delay) {
    std::lock_guard<std::mutex> lock(queueMutex);

    Task task;
    task.command = command;
    task.executeAt = std::chrono::system_clock::now() + std::chrono::seconds(delay);
    taskQueue.push(task);

    cv.notify_one();
}

void TaskScheduler::run() {
    std::thread(&TaskScheduler::processTasks, this).detach();
}

void TaskScheduler::processTasks() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (taskQueue.empty()) {
            cv.wait(lock); // Wait for tasks
        }

        while (!taskQueue.empty()) {
            auto now = std::chrono::system_clock::now();
            if (taskQueue.top().executeAt <= now) {
                Task task = taskQueue.top();
                taskQueue.pop();
                lock.unlock();

                std::cout << "Executing: " << task.command << std::endl;
                int result = std::system(task.command.c_str());
                if (result != 0) {
                    std::cerr << "Command failed: " << task.command << std::endl;
                }

                lock.lock();
            } else {
                cv.wait_until(lock, taskQueue.top().executeAt);
            }
        }
    }
}