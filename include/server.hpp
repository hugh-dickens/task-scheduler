#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <netinet/in.h>
#include <unistd.h>
#include "scheduler.hpp"

class TaskServer {
    public:
        TaskServer(int port, ITaskScheduler& scheduler);
        void start();
        
    private:
        int serverSocket;
        void handleClient(int clientSocket);

        ITaskScheduler& scheduler;
};