#pragma once
#include <string>

#include "file_conversions/csv_handler.hpp"
#include "tasks.hpp"  // Contains ITask and Logger declarations

class FileConversionTask : public ITask {
   public:
    explicit FileConversionTask(const std::string& filePath, const std::string& targetFormat);
    void run(Logger& logger) override;

   private:
    std::string filePath;
    std::string targetFormat;
    std::vector<std::unique_ptr<IFileHandler>> handlers;
};
