#include "gtest/gtest.h"
#include "cpp/log/logger.h"

TEST(LogTest, Print) {
    INFO("hello {}", "world");

    cpp::log::Log(cpp::log::Level::Debug, "logger_test.cpp", 7, "", "hello {}", "1111");
}