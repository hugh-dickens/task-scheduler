#include "server.hpp"
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

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to bind server socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Failed to listen on server socket");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;
}

void TaskServer::start() {
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        std::thread clientThread(&TaskServer::handleClient, this, clientSocket);
        clientThread.detach();
    }    
}

void TaskServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    
    if (bytesRead == -1) {
        perror("Failed to read from client socket");
        close(clientSocket);
        return;
    }

    if (bytesRead > 0) {
        std::string request(buffer, bytesRead);
        std::cout << "[DEBUG] Received task request: " << request << std::endl;

        // Parse request format: "command|delay"
        size_t delimiterPos = request.find('|');
        if (delimiterPos != std::string::npos) {
            std::string command = request.substr(0, delimiterPos);
            int delay = std::stoi(request.substr(delimiterPos + 1));

            scheduler.scheduleTask(command, delay);
            std::string response = "Task scheduled: " + command + "\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    close(clientSocket);
}