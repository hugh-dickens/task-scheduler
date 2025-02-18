#include "file_conversions/json_handler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using json = nlohmann::json;

bool JsonHandler::supports(const std::string& filePath) const {
    return filePath.ends_with(".json");
}

std::unique_ptr<IFileHandler> JsonHandler::clone() const {
    return std::make_unique<JsonHandler>();
}

std::vector<std::vector<std::string>> JsonHandler::read(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR] Failed to open JSON file: " << filePath << "\n";
        return {};
    }

    json jsonArray;
    try {
        inputFile >> jsonArray;
    } catch (const json::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << "\n";
        return {};
    }

    if (!jsonArray.is_array() || jsonArray.empty()) {
        std::cerr << "[ERROR] JSON file must contain an array of objects\n";
        return {};
    }

    std::vector<std::vector<std::string>> table;
    std::vector<std::string> headers;

    // Extract headers
    for (const auto& [key, _] : jsonArray[0].items()) {
        headers.push_back(key);
    }
    table.push_back(headers);

    // Extract rows
    for (const auto& item : jsonArray) {
        std::vector<std::string> row;
        for (const auto& header : headers) {
            row.push_back(item.value(header, ""));
        }
        table.push_back(row);
    }

    return table;
}

void JsonHandler::write(const std::string& filePath, const std::vector<std::vector<std::string>>& data) {
    if (data.empty()) {
        std::cerr << "[ERROR] No data provided for JSON conversion\n";
        return;
    }

    json jsonArray;
    std::vector<std::string> headers = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        json jsonObject;
        for (size_t j = 0; j < headers.size(); ++j) {
            jsonObject[headers[j]] = data[i][j];
        }
        jsonArray.push_back(jsonObject);
    }

    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "[ERROR] Failed to create output JSON file: " << filePath << "\n";
        return;
    }

    outputFile << jsonArray.dump(4);
    std::cout << "[INFO] Successfully wrote JSON file: " << filePath << "\n";
}
