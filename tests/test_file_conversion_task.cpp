#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include "file_conversion_task.hpp"
#include "fake_logger.hpp"
#include <vector>
#include <string>

// Test fixture that creates and cleans up a temporary directory for test files.
class FileConversionTaskTest : public ::testing::Test {
protected:
    std::filesystem::path tempDir;

    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() / "test_conversion_temp";
        std::filesystem::create_directory(tempDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(tempDir);
    }

    // Helper to read file content.
    std::string readFile(const std::filesystem::path& path) {
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        return buffer.str();
    }
};

// Test JSON to CSV conversion expecting header "age,name"
TEST_F(FileConversionTaskTest, JsonToCsvConversion) {
    // Setup file paths.
    std::filesystem::path inputJsonPath = tempDir / "temp_test.json";
    std::filesystem::path outputCsvPath = tempDir / "temp_test.csv"; // Expected output file path

    // Write a simple JSON array to the input file.
    {
        std::ofstream ofs(inputJsonPath);
        ofs << R"([
            {"name": "Alice", "age": "30"},
            {"name": "Bob", "age": "25"}
        ])";
    }

    FakeLogger fakeLogger;
    FileConversionTask task(inputJsonPath.string());
    task.run(fakeLogger);

    // Wait a brief moment for file I/O to complete.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Read the generated CSV file.
    std::string csvContent = readFile(outputCsvPath);

    EXPECT_NE(csvContent.find("age,name"), std::string::npos) << "CSV header not as expected: " << csvContent;
    EXPECT_NE(csvContent.find("Alice"), std::string::npos) << "Alice not found in CSV";
    EXPECT_NE(csvContent.find("Bob"), std::string::npos) << "Bob not found in CSV";

    ASSERT_FALSE(fakeLogger.logs.empty());
    EXPECT_NE(fakeLogger.logs.front().find("Converted JSON to CSV"), std::string::npos)
        << "Expected conversion log not found";
}

// Test CSV to JSON conversion (unchanged)
TEST_F(FileConversionTaskTest, CsvToJsonConversion) {
    // Setup file paths.
    std::filesystem::path inputCsvPath = tempDir / "temp_test.csv";
    std::filesystem::path outputJsonPath = tempDir / "temp_test.json";

    // Write a simple CSV file with header "age,name"
    {
        std::ofstream ofs(inputCsvPath);
        ofs << "age,name\n";
        ofs << "\"30\",\"Alice\"\n";
        ofs << "\"25\",\"Bob\"\n";
    }

    FakeLogger fakeLogger;
    FileConversionTask task(inputCsvPath.string());
    task.run(fakeLogger);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Read the generated JSON file.
    std::string jsonContent = readFile(outputJsonPath);

    EXPECT_NE(jsonContent.find("\"age\""), std::string::npos) << "Key 'age' missing in JSON";
    EXPECT_NE(jsonContent.find("\"Alice\""), std::string::npos) << "Alice not found in JSON";
    EXPECT_NE(jsonContent.find("\"Bob\""), std::string::npos) << "Bob not found in JSON";

    ASSERT_FALSE(fakeLogger.logs.empty());
    EXPECT_NE(fakeLogger.logs.front().find("Converted CSV to JSON"), std::string::npos)
        << "Expected conversion log not found";
}
