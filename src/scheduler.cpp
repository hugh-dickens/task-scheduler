#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <json.hpp>

#include "scheduler.hpp"
#include "logger.hpp"

using json = nlohmann::json;

void TaskScheduler::scheduleTask(const std::string& input, int delay, TaskType type) {
    std::lock_guard<std::mutex> lock(queueMutex);

    Task task;
    task.type = type;
    std::cout << "[DEBUG] Task type: " << task.type << "\n";

    task.executeAt = std::chrono::system_clock::now() + std::chrono::seconds(delay);

    if (type == TaskType::COMMAND) {
        task.command = input;
    } else if (type == TaskType::FILE_PROCESS) {
        task.filePath = input;
    }

    taskQueue.push(task);
    taskCondition.notify_one();
}

void TaskScheduler::run() { std::thread(&TaskScheduler::processTasks, this).detach(); }

void TaskScheduler::processFile(const std::string& filePath) {
    std::ifstream inputFile(filePath, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << filePath << "\n";
        return;
    }

    std::string outputFilePath = filePath + ".json";
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "[ERROR] Failed to create output file: " << outputFilePath << "\n";
        return;
    }

    std::string line;
    std::vector<std::string> headers;
    json jsonArray = json::array();

    // Read the header line
    if (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string column;
        while (std::getline(ss, column, ',')) {
            headers.push_back(column);
        }
    }

    // Read the data lines
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string value;
        json jsonObject;
        size_t colIndex = 0;

        while (std::getline(ss, value, ',')) {
            if (colIndex < headers.size()) {
                jsonObject[headers[colIndex]] = value;
            }
            colIndex++;
        }

        jsonArray.push_back(jsonObject);
    }
    
    // write JSON to file
    outputFile << jsonArray.dump(4);

    inputFile.close();
    outputFile.close();
    std::cout << "[INFO] CSV converted to JSON: " << outputFilePath << "\n";
}

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

            if (task.type == TaskType::COMMAND) {
                std::cout << "[INFO] Executing command: " << task.command << "\n";
                int result = std::system(task.command.c_str());
                if (result != 0) {
                    std::cerr << "[ERROR] Command failed: " << task.command << "\n";
                    Logger::getInstance().log("Task failed: " + task.command);
                } else {
                    Logger::getInstance().log("Task completed: " + task.command);
                }
            } else if (task.type == TaskType::FILE_PROCESS) {
                std::cout << "[INFO] Processing file: " << task.filePath << "\n";
                processFile(task.filePath);
                Logger::getInstance().log("Processed file: " + task.filePath);
            }

            lock.lock();
        }
    }
}