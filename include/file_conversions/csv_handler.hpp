#pragma once
#include "file_handler.hpp"

class CSVHandler : public IFileHandler {
public:
    bool supports(const std::string& filePath) const override;
    std::vector<std::vector<std::string>> read(const std::string& filePath) override;
    void write(const std::string& filePath, const std::vector<std::vector<std::string>>& data) override;
    std::unique_ptr<IFileHandler> clone() const override;
};

