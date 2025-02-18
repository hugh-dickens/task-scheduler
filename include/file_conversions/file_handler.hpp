#pragma once
#include <string>
#include <vector>
#include <memory>

// standard for all format handlers. Allows polymorphism and keeps the task scheduler modular
class IFileHandler {
    public:
        virtual ~IFileHandler() = default;
    
        virtual bool supports(const std::string& filePath) const = 0;
        virtual std::vector<std::vector<std::string>> read(const std::string& filePath) = 0;
        virtual void write(const std::string& filePath, const std::vector<std::vector<std::string>>& data) = 0;
        virtual std::unique_ptr<IFileHandler> clone() const = 0;
    };