#include "file_conversion_task.hpp"

#include <filesystem>
#include <iostream>

#include "file_conversions/csv_handler.hpp"
#include "file_conversions/excel_handler.hpp"
#include "file_conversions/json_handler.hpp"

FileConversionTask::FileConversionTask(const std::string& filePath, const std::string& targetFormat)
    : filePath(filePath), targetFormat(targetFormat) {
    handlers.emplace_back(std::make_unique<CSVHandler>());
    handlers.emplace_back(std::make_unique<ExcelHandler>());
    handlers.emplace_back(std::make_unique<JsonHandler>());
}

void FileConversionTask::run(Logger& logger) {
    std::string inputExtension = std::filesystem::path(filePath).extension().string();
    std::unique_ptr<IFileHandler> inputHandler = nullptr;
    std::unique_ptr<IFileHandler> outputHandler = nullptr;

    // Find the correct handler for reading the input file
    for (const auto& handler : handlers) {
        if (handler->supports(filePath)) {
            inputHandler =
                handler->clone();  // Clone the handler to avoid modifying the stored handlers
            break;
        }
    }

    if (!inputHandler) {
        logger.log("[ERROR] Unsupported input file format: " + inputExtension);
        return;
    }

    std::vector<std::vector<std::string>> data = inputHandler->read(filePath);

    std::string outputFilePath =
        filePath.substr(0, filePath.find_last_of(".")) + "." + targetFormat;

    // Find the correct handler for writing to the target format
    for (const auto& handler : handlers) {
        if (handler->supports(outputFilePath)) {
            outputHandler = handler->clone();
            break;
        }
    }

    if (!outputHandler) {
        logger.log("[ERROR] Unsupported target file format: " + targetFormat);
        return;
    }

    outputHandler->write(outputFilePath, data);
    logger.log("[INFO] Successfully converted " + filePath + " -> " + outputFilePath);
}
