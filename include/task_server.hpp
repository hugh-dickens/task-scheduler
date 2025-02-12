#pragma once
#include <netinet/in.h>
#include <unistd.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

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