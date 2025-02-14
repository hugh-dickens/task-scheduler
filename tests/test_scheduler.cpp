#include <gtest/gtest.h>
#include "scheduler.hpp"
#include "fake_logger.hpp"
#include <chrono>
#include <thread>
#include <vector>
#include <string>


// TestTask: a simple task that logs a given message when run.
class TestTask : public ITask {
public:
    explicit TestTask(const std::string& msg) : message(msg) {}
    void run(Logger& logger) override {
        logger.log(message);
    }
private:
    std::string message;
};

// Test case: verifies that a scheduled task executes and logs its message.
TEST(TaskSchedulerTest, ExecutesScheduledTask) {
    FakeLogger fakeLogger;
    TaskScheduler scheduler(fakeLogger);
    
    // Schedule a task with a 1-second delay.
    std::string expectedMessage = "Task executed";
    scheduler.scheduleTask(std::make_unique<TestTask>(expectedMessage), 1);
    
    // Start the scheduler in a background thread.
    scheduler.run();
    
    // Wait long enough for the task to execute.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Stop the scheduler to clean up.
    scheduler.stop();
    
    // Verify that the logger recorded the expected message.
    ASSERT_FALSE(fakeLogger.logs.empty());
    EXPECT_EQ(fakeLogger.logs.front(), expectedMessage);
}

// Test case: verifies that tasks are executed in order based on delay.
TEST(TaskSchedulerTest, ProcessesTasksInOrder) {
    FakeLogger fakeLogger;
    TaskScheduler scheduler(fakeLogger);
    
    // Schedule two tasks with different delays.
    std::string msgFirst = "First task";
    std::string msgSecond = "Second task";
    scheduler.scheduleTask(std::make_unique<TestTask>(msgSecond), 2);
    scheduler.scheduleTask(std::make_unique<TestTask>(msgFirst), 1);
    
    scheduler.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    scheduler.stop();
    
    // Verify that the first logged message corresponds to the task with the shorter delay.
    ASSERT_GE(fakeLogger.logs.size(), 2u);
    EXPECT_EQ(fakeLogger.logs[0], msgFirst);
    EXPECT_EQ(fakeLogger.logs[1], msgSecond);
}
