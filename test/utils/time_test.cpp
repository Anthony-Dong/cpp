#include "gtest/gtest.h"
#include "cpp/utils/time.h"

using namespace cpp::utils;
TEST(UtilsTest, TimeTest) {
    auto time = FormatCurrentTime();
    std::cout << time + "---" + "end" << std::endl;
};