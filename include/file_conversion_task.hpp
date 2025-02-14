#pragma once
#include "tasks.hpp"  // Contains ITask and Logger declarations
#include <string>

class FileConversionTask : public ITask {
public:
    explicit FileConversionTask(const std::string& filePath);
    void run(Logger& logger) override;

private:
    std::string filePath;

    void convertJsonToCsv(const std::string& jsonFilePath, const std::string& csvFilePath);
    void convertCsvToJson(const std::string& csvFilePath, const std::string& jsonFilePath);
};
