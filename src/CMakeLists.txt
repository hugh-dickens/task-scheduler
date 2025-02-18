find_package(OpenXLSX REQUIRED)  # Excel file conversions

# Define the TaskScheduler library
add_library(TaskScheduler STATIC 
    task_server.cpp
    scheduler.cpp
    logger.cpp
    security.cpp
    command_task.cpp
    file_conversion_task.cpp 
    file_conversions/csv_handler.cpp
    file_conversions/excel_handler.cpp
    file_conversions/json_handler.cpp
)

# Link necessary libraries
target_link_libraries(TaskScheduler 
    PRIVATE OpenXLSX   # OpenXLSX is only used internally in TaskScheduler
    PRIVATE pthread    # Threading support
)

# Include necessary directories
target_include_directories(TaskScheduler 
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/include/file_conversions
    PRIVATE /usr/local/include/OpenXLSX
    PRIVATE /usr/local/include/OpenXLSX/headers  # Fixes missing XLException.hpp error
)

# Define the Server executable
add_executable(Server main_server.cpp)

# Set output directory for the Server binary
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

# Link TaskScheduler with Server
target_link_libraries(Server PRIVATE TaskScheduler)