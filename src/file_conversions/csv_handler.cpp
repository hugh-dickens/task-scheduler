#include "file_conversions/csv_handler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool CSVHandler::supports(const std::string& filePath) const {
    return filePath.ends_with(".csv");
}

std::unique_ptr<IFileHandler> CSVHandler::clone() const {
    return std::make_unique<CSVHandler>();
}

std::vector<std::vector<std::string>> CSVHandler::read(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    std::vector<std::vector<std::string>> data;
    
    if (!inputFile) {
        std::cerr << "[ERROR] Failed to open CSV file: " << filePath << std::endl;
        return {};
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    return data;
}

void CSVHandler::write(const std::string& filePath, const std::vector<std::vector<std::string>>& data) {
    std::ofstream outputFile(filePath);
    if (!outputFile) {
        std::cerr << "[ERROR] Could not create output CSV file." << std::endl;
        return;
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            outputFile << row[i] << (i < row.size() - 1 ? "," : "\n");
        }
    }

    std::cout << "[INFO] Successfully wrote to CSV: " << filePath << std::endl;
}
