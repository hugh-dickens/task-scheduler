// server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <netinet/in.h>
#include <unistd.h>

class TaskServer {
    public:
        TaskServer(int port);
        void start();
        
    private:
        void handleClient(int clientSocket);
        int serverSocket;
};

#endif // SERVER_HPP