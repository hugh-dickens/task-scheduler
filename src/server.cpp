#include "server.hpp"

#include <array>
#include <sstream>

#include "scheduler.hpp"

TaskServer::TaskServer(int port, ITaskScheduler& scheduler) : scheduler(scheduler) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create server socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) ==
        -1) {
        perror("Failed to bind server socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Failed to listen on server socket");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "\n";
}

void TaskServer::start() {
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket =
            accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientLen);

        if (clientSocket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        std::thread clientThread(&TaskServer::handleClient, this, clientSocket);
        clientThread.detach();
    }
}

void TaskServer::handleClient(int clientSocket) {
    std::array<char, 1024> buffer = {0};
    int bytesRead = recv(clientSocket, buffer.data(), sizeof(buffer), 0);

    if (bytesRead == -1) {
        perror("Failed to read from client socket");
        close(clientSocket);
        return;
    }

    if (bytesRead > 0) {
        std::string request(buffer.data(), bytesRead);

        std::istringstream requestStream(request);
        std::string taskType, taskInput, delayStr;

        if (std::getline(requestStream, taskType, '|') &&
            std::getline(requestStream, taskInput, '|') && std::getline(requestStream, delayStr)) {
            try {
                int delay = std::stoi(delayStr);

                if (taskType == "COMMAND") {
                    scheduler.scheduleTask(taskInput, delay, TaskType::COMMAND);
                } else if (taskType == "FILE_PROCESS") {
                    scheduler.scheduleTask(taskInput, delay, TaskType::FILE_PROCESS);
                } else {
                    std::string response = "Invalid task type\n";
                    send(clientSocket, response.c_str(), response.size(), 0);
                    close(clientSocket);
                    return;
                }

                std::string response = "Task scheduled: " + taskInput + "\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            } catch (const std::exception& e) {
                std::string response = "Error processing request: " + std::string(e.what()) + "\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        } else {
            std::string response = "Invalid request format\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }

    close(clientSocket);
}