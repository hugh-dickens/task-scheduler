cmake_minimum_required(VERSION 3.10)
project(TaskSchedulerProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Include header files
include_directories(include)

# Server executable
add_executable(Server src/main_server.cpp src/server.cpp src/scheduler.cpp)
target_link_libraries(Server pthread)

# Client executable
add_executable(Client src/main_client.cpp src/client.cpp)
target_link_libraries(Client pthread)
