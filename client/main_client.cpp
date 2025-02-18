#include "task_client.hpp"
#include <iostream>
#include <string>

int main() {
    std::string serverIP = "127.0.0.1";
    int port = 8080;

    TaskClient client(serverIP, port);

    std::string taskInput;
    std::string taskType;
    std::string targetFormat;
    int delay;

    int choice;
    std::cout << "Enter 1 for a COMMAND or 2 to process a FILE: ";
    std::cin >> choice;
    std::cin.ignore();  // Clear the input buffer

    if (choice == 1) {
        taskType = "COMMAND";
        std::cout << "Enter command: ";
        std::getline(std::cin, taskInput);
    } else if (choice == 2) {
        taskType = "FILE_PROCESS";
        std::cout << "Enter file path: ";
        std::getline(std::cin, taskInput);

        std::cout << "Enter the target file format (csv, json, xlsx): ";
        std::getline(std::cin, targetFormat);
    } else {
        std::cout << "Invalid choice\n";
        return 1;
    }

    std::cout << "Enter delay in seconds: ";
    std::cin >> delay;

    // Send the task input along with the target format if it's a file conversion
    std::string fullTaskInput = (taskType == "FILE_PROCESS") ? (taskInput + "|" + targetFormat) : taskInput;

    client.sendTask(fullTaskInput, delay, taskType);

    return 0;
}

