#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class TaskClient {
    public:
        TaskClient(const std::string& serverIP, int port);
        void sendTask(const std::string& taskCommand, int delay);
        
    private:
        int clientSocket;
};

#endif // CLIENT_HPP