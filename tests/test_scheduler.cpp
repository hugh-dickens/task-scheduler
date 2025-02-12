#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "scheduler.hpp"
#include "json.hpp"

using json = nlohmann::json;

class TaskSchedulerTest : public ::testing::Test {
protected:
    TaskScheduler scheduler;

    void SetUp() override {
        // Create test files before each test
        createTestCsv();
        createTestJson();
    }

    void TearDown() override {
        // Cleanup test files after each test
        remove("test.csv");
        remove("test.json");
        remove("output.csv");
        remove("output.json");
    }

    void createTestCsv() {
        std::ofstream file("test.csv");
        file << "age,city,name\n";
        file << "30,New York,Alice\n";
        file << "25,Los Angeles,Bob\n";
        file.close();
    }
    
    void createTestJson() {
        json testData = {
            {{"age", "30"}, {"city", "New York"}, {"name", "Alice"}},
            {{"age", "25"}, {"city", "Los Angeles"}, {"name", "Bob"}}
        };
        std::ofstream file("test.json");
        file << testData.dump(4);
        file.close();
    }
    
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};
    
// Test JSON → CSV conversion
TEST_F(TaskSchedulerTest, ConvertJsonToCsv) {
    scheduler.testConvertJsonToCsv("test.json", "output.csv");

    ASSERT_TRUE(fileExists("output.csv"));

    std::ifstream file("output.csv");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string expected =
        "age,city,name\n"
        "\"30\",\"New York\",\"Alice\"\n"
        "\"25\",\"Los Angeles\",\"Bob\"\n";

    ASSERT_EQ(buffer.str(), expected);
}

// Test CSV → JSON conversion
TEST_F(TaskSchedulerTest, ConvertCsvToJson) {
    scheduler.testConvertCsvToJson("test.csv", "output.json");

    ASSERT_TRUE(fileExists("output.json"));

    std::ifstream file("output.json");
    json outputJson;
    file >> outputJson;
    file.close();

    json expected = {
        {{"age", "30"},{"city", "New York"},{"name", "Alice"}},
        {{"age", "25"},{"city", "Los Angeles"},{"name", "Bob"}}
    };

    ASSERT_EQ(outputJson, expected);
}

// Test scheduling a command task
TEST_F(TaskSchedulerTest, ScheduleCommandTask) {
    scheduler.scheduleTask("echo Hello", 1, TaskType::COMMAND);

    ASSERT_FALSE(scheduler.isTaskQueueEmpty());

    Task task = scheduler.getTopTask();
    ASSERT_EQ(task.type, TaskType::COMMAND);
    ASSERT_EQ(task.command, "echo Hello");
}

// Test scheduling a file processing task
TEST_F(TaskSchedulerTest, ScheduleFileProcessingTask) {
    scheduler.scheduleTask("test.csv", 2, TaskType::FILE_PROCESS);

    ASSERT_FALSE(scheduler.isTaskQueueEmpty());

    Task task = scheduler.getTopTask();
    ASSERT_EQ(task.type, TaskType::FILE_PROCESS);
    ASSERT_EQ(task.filePath, "test.csv");
}
