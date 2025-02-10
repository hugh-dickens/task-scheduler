#include "client.hpp"

int main() {
    std::string serverIP = "127.0.0.1";
    int port = 8080;

    TaskClient client(serverIP, port);

    std::string taskInput;
    int delay;
    std::string taskType;

    int choice;
    std::cout << "Enter 1 for a COMMAND or 2 to process a FILE: ";
    std::cin >> choice;

    if (choice == 1) {
        taskType = "COMMAND";
        std::cout << "Enter command: ";
        std::cin >> taskInput;
    } else if (choice == 2) {
        taskType = "FILE_PROCESS";
        std::cout << "Enter file path: ";
        std::cin >> taskInput;
    } else {
        std::cout << "Invalid choice\n";
        return 1;
    }

    std::cout << "Enter delay in seconds: ";
    std::cin >> delay;

    client.sendTask(taskInput, delay, taskType);

    return 0;
}
