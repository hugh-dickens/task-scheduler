#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Define an interface with a virtual function
class ICalculator {
   public:
    virtual ~ICalculator() = default;
    virtual int add(int a, int b) = 0;
};

// Mock class using Google Mock
class MockCalculator : public ICalculator {
   public:
    MOCK_METHOD(int, add, (int a, int b), (override));
};

// Test fixture using Google Test
class MockCalculatorTest : public ::testing::Test {
   protected:
    MockCalculator mockCalculator;
};

// Simple test to verify Google Test and Google Mock work
TEST_F(MockCalculatorTest, AddTest) {
    // Expect that add(3, 4) is called once and returns 7
    EXPECT_CALL(mockCalculator, add(3, 4)).WillOnce(::testing::Return(7));

    // Act
    int result = mockCalculator.add(3, 4);

    // Assert
    EXPECT_EQ(result, 7);
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
