add_library(TaskScheduler STATIC 
    task_server.cpp
    scheduler.cpp
    logger.cpp
    security.cpp
    task.cpp 
)

target_include_directories(TaskScheduler PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Server executable
add_executable(Server 
    main_server.cpp 
)
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
target_link_libraries(Server PRIVATE TaskScheduler pthread)