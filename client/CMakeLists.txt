add_library(ClientLib STATIC task_client.cpp)

target_include_directories(ClientLib PUBLIC ${CMAKE_SOURCE_DIR}/client)

# Create the Client executable
add_executable(Client main_client.cpp)

# Set output directory
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

target_link_libraries(Client PRIVATE ClientLib pthread)