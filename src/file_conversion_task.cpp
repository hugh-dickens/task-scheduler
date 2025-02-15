#include "file_conversion_task.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <vector>

using json = nlohmann::json;

FileConversionTask::FileConversionTask(const std::string& filePath) : filePath(filePath) {}

void FileConversionTask::run(Logger& logger) {
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    if (fileExtension == ".json") {
        std::string outputFilePath = filePath.substr(0, filePath.find_last_of(".")) + ".csv";
        convertJsonToCsv(filePath, outputFilePath);
        logger.log("Converted JSON to CSV: " + outputFilePath);
    } else if (fileExtension == ".csv") {
        std::string outputFilePath = filePath.substr(0, filePath.find_last_of(".")) + ".json";
        convertCsvToJson(filePath, outputFilePath);
        logger.log("Converted CSV to JSON: " + outputFilePath);
    } else {
        logger.log("Unsupported file format: " + fileExtension);
    }
}

void FileConversionTask::convertJsonToCsv(const std::string& jsonFilePath,
                                          const std::string& csvFilePath) {
    std::ifstream inputFile(jsonFilePath);
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

    // Write CSV headers
    for (size_t i = 0; i < headers.size(); i++) {
        outputFile << headers[i] << (i < headers.size() - 1 ? "," : "\n");
    }

    // Write CSV data
    for (const auto& item : jsonArray) {
        for (size_t i = 0; i < headers.size(); i++) {
            std::string value = item.value(headers[i], "");
            outputFile << "\"" << value << "\"" << (i < headers.size() - 1 ? "," : "\n");
        }
    }
}

void FileConversionTask::convertCsvToJson(const std::string& csvFilePath,
                                          const std::string& jsonFilePath) {
    std::ifstream inputFile(csvFilePath);
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

    // Read header line
    if (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string column;
        while (std::getline(ss, column, ',')) {
            headers.push_back(column);
        }
    }

    // Read data lines -> greater robustness required
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string value;
        json jsonObject;
        for (size_t i = 0; i < headers.size(); i++) {
            if (!std::getline(ss, value, ',')) {
                value = "";  // Handle missing values
            }
            // Remove surrounding quotes if they exist
            if (!value.empty() && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }
            jsonObject[headers[i]] = value;
        }
        jsonArray.push_back(jsonObject);
    }

    outputFile << jsonArray.dump(4);
}
