#include <gtest/gtest.h>
#include "command_task.hpp"
#include "fake_logger.hpp"
#include <vector>
#include <string>

// Test that CommandTask logs "Task completed: <command>" when the command succeeds.
TEST(CommandTaskTest, SucceedsForEcho) {
    FakeLogger fakeLogger;
    // "echo Hello" should succeed on most Unix systems.
    CommandTask task("echo Hello");
    task.run(fakeLogger);
    
    // Verify that the fake logger received the "completed" log.
    ASSERT_FALSE(fakeLogger.logs.empty());
    EXPECT_EQ(fakeLogger.logs[0], "Task completed: echo Hello");
}

// Test that CommandTask logs "Task failed: <command>" when the command fails.
TEST(CommandTaskTest, FailsForFalse) {
    FakeLogger fakeLogger;
    // "false" is a command that returns nonzero on Unix.
    CommandTask task("false");
    task.run(fakeLogger);
    
    // Verify that the fake logger received the "failed" log.
    ASSERT_FALSE(fakeLogger.logs.empty());
    EXPECT_EQ(fakeLogger.logs[0], "Task failed: false");
}