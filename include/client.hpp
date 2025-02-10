#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class TaskClient {
   public:
    TaskClient(const std::string& serverIP, int port);
    void sendTask(const std::string& taskInput, int delay, const std::string& taskType);

   private:
    int clientSocket;
};