#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>

struct Task {
    std::string command;
    std::chrono::time_point<std::chrono::system_clock> executeAt;
    
    bool operator<(const Task& other) const {
        return executeAt > other.executeAt;
    }
};

class TaskScheduler {
    public:
        void scheduleTask(const std::string& command, int delay);
        void run();
        
    private:
    std::priority_queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool running = true;
    void processTasks();
};
#endif // SCHEDULER_HPP