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
    
    std::string fileExtension = std::filesystem::path(filePath).extension().string();

    if (fileExtension == ".json") {
        std::string outputFilePath = filePath.substr(0, filePath.find_last_of(".")) + ".csv";
        convertJsonToCsv(filePath, outputFilePath);
    } else if (fileExtension == ".csv") {
        std::string outputFilePath = filePath.substr(0, filePath.find_last_of(".")) + ".json";
        convertCsvToJson(filePath, outputFilePath);
    } else {
        std::cerr << "[ERROR] Unsupported file format: " << fileExtension << "\n";
    }       
}

void TaskScheduler::convertJsonToCsv(const std::string& jsonFilePath, const std::string& csvFilePath) {
    std::ifstream inputFile(jsonFilePath, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << jsonFilePath << "\n";
        return;
    }

    std::ofstream outputFile(csvFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "[ERROR] Failed to create output file: " << csvFilePath << "\n";
        return;
    }

    json jsonArray;
    try {
        inputFile >> jsonArray;
    } catch (const json::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << "\n";
        return;
    }

    if (!jsonArray.is_array() || jsonArray.empty()) {
        std::cerr << "[ERROR] JSON file must contain an array of objects\n";
        return;
    }

    std::vector<std::string> headers;
    for (const auto& [key, _] : jsonArray[0].items()) {
        headers.push_back(key);
    }

    // Write csv headers
    for (size_t i = 0; i < headers.size(); i++) {
        outputFile << headers[i];
        if (i < headers.size() - 1) {
            outputFile << ",";
        }
    }
    outputFile << "\n";

    // Write csv data
    for (const auto& item : jsonArray) {
        for (size_t i = 0; i < headers.size(); i++) {
            std::string value = item.value(headers[i], "");
            outputFile << "\"" << value << "\"";
            if (i < headers.size() - 1) {
                outputFile << ",";
            }
            outputFile << "\n";
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "[INFO] JSON converted to CSV: " << csvFilePath << "\n";
}

void TaskScheduler::convertCsvToJson(const std::string& csvFilePath, const std::string& jsonFilePath) {
    std::ifstream inputFile(csvFilePath, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << csvFilePath << "\n";
        return;
    }

    std::ofstream outputFile(jsonFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "[ERROR] Failed to create output file: " << jsonFilePath << "\n";
        return;
    }

    std::string line;
    std::vector<std::string> headers;
    json jsonArray;

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

        for (size_t i = 0; i < headers.size(); i++) {
            if (!std::getline(ss, value, ',')) {
                value = ""; // Handle missing values
            }
            jsonObject[headers[i]] = value;
        }
        jsonArray.push_back(jsonObject);
    }
    
    // write JSON to file
    outputFile << jsonArray.dump(4);

    inputFile.close();
    outputFile.close();
    std::cout << "[INFO] CSV converted to JSON: " << jsonFilePath << "\n";
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