#include "gtest/gtest.h"

#include "listener.h"
#include "cpp/utils/time.h"

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(NetworkTest, CreateListener) {
//    auto socket = cpp::network::listener::NewTCPListener(10086, false);
}

TEST(NetworkTest, UtilsTest) {
    std::cout << cpp::utils::FormatCurrentTime() << std::endl;
}