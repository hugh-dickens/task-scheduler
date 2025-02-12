cmake_minimum_required(VERSION 3.10)
project(TaskSchedulerProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(client)
add_subdirectory(tests)

# Enable clang-tidy if available
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=*")
endif()

# Collect all .cpp files in the src/ directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Custom target to manually run clang-tidy
add_custom_target(
    run_clang_tidy
    COMMAND ${CLANG_TIDY_EXE} ${SOURCE_FILES} -- -I${CMAKE_SOURCE_DIR}/include ${CMAKE_CXX_FLAGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-tidy on source files"
    VERBATIM
)

# Enable cppcheck if available
find_program(CPPCHECK_EXE NAMES cppcheck)
if(CPPCHECK_EXE)
    add_custom_target(
        run_cppcheck
        COMMAND ${CPPCHECK_EXE} --enable=all --inconclusive --quiet
        --language=c++
        --std=c++20
        --suppress=missingIncludeSystem
        -I ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running cppcheck on source files"
        VERBATIM
    )
endif()
