#include "scheduler.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

TaskScheduler::TaskScheduler(Logger& logger) : logger(logger), running(true) {}

TaskScheduler::~TaskScheduler() { cleanup(); }

void TaskScheduler::cleanup() {
    if (running) {
        running = false;
        taskCondition.notify_all();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

void TaskScheduler::scheduleTask(std::unique_ptr<ITask> task, int delay) {
    auto executeTime = std::chrono::system_clock::now() + std::chrono::seconds(delay);
    ScheduledTask scheduled{executeTime, std::move(task)};

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskHeap.push_back(std::move(scheduled));
        std::push_heap(taskHeap.begin(), taskHeap.end());
    }
    taskCondition.notify_one();
}

void TaskScheduler::run() {
    if (!workerThread.joinable()) {
        workerThread = std::thread(&TaskScheduler::processTasks, this);
    }
}

void TaskScheduler::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running = false;
    }
    taskCondition.notify_all();
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void TaskScheduler::processTasks() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);

        // Wait until there's a task or we're stopping.
        taskCondition.wait(lock, [this] { return !taskHeap.empty() || !running; });
        if (!running && taskHeap.empty()) break;

        auto now = std::chrono::system_clock::now();

        // The top element of the heap is at front.
        if (taskHeap.front().executeAt > now) {
            taskCondition.wait_until(lock, taskHeap.front().executeAt);
            continue;
        }

        // Remove the top element from the heap.
        std::pop_heap(taskHeap.begin(), taskHeap.end());
        ScheduledTask scheduled = std::move(taskHeap.back());
        taskHeap.pop_back();
        lock.unlock();

        auto taskToRun = std::move(scheduled.task);
        if (taskToRun) {
            taskToRun->run(logger);
        }
    }
}
