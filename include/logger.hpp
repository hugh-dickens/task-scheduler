#pragma once
#include <fstream>
#include <mutex>
#include <string>

class Logger {
   public:
    static Logger& getInstance();
    virtual void log(const std::string& message);
    virtual ~Logger();

   protected:
    Logger();

    std::ofstream logFile;
    std::mutex logMutex;
};