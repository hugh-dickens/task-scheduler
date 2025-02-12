find_package(GTest REQUIRED)

add_executable(run_tests 
    test_imports.cpp
    test_scheduler.cpp
)

target_include_directories(run_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(run_tests 
    PRIVATE 
    TaskScheduler
    GTest::GTest 
    GTest::Main 
    GTest::gmock 
    GTest::gmock_main 
    pthread
)
add_test(NAME TaskSchedulerTests COMMAND run_tests)