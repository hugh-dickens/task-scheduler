#include "client.hpp"

TaskClient::TaskClient(const std::string& serverIP, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create client socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }
}

void TaskClient::sendTask(const std::string& taskCommand, int delay) {
    std::string msg = taskCommand + "|" + std::to_string(delay);
    send(clientSocket, msg.c_str(), msg.size(), 0);

    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0) {
        std::cout << "Server response: " << std::string(buffer, bytesRead) << std::endl;
    }

    close(clientSocket);
}
