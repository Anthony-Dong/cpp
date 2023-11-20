#include "gtest/gtest.h"
#include "io.h"
#include "cpp/utils/os.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(FileTest, FileTestIO) {
    using namespace cpp;
    auto filename = utils::GetPWD() + "/io_test.cpp";
    std::cout << filename << std::endl;
    io::FileReadWriter file(filename);
    std::string buffer{};
    buffer.resize(1024);
    auto size = file.read(buffer);
    std::cout << size << std::endl;
    std::cout << buffer << std::endl;
}

TEST(FileTest, PWD) {
    std::cout << cpp::utils::GetPWD() << std::endl;
}