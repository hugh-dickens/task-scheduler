#include "client.hpp"

int main() {
    std::string serverIP = "127.0.0.1";
    int port = 8080;

    TaskClient client(serverIP, port);

    std::string taskCommand;
    int delay;

    std::cout << "Enter command to schedule: ";
    std::getline(std::cin, taskCommand);

    std::cout << "Enter delay in seconds: ";
    std::cin >> delay;

    client.sendTask(taskCommand, delay);

    return 0;
}
