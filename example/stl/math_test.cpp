#include "fmt/core.h"
#include "gtest/gtest.h"
#include <iostream>
#include <random>

int rand_int(int max) {
    std::random_device seed;                         // 硬件生成随机数种子
    std::ranlux48 engine(seed());                    // 利用种子生成随机数引擎
    std::uniform_int_distribution<> distrib(0, max); // 设置随机数范围，并为均匀分布
    return distrib(engine);
}

TEST(rand, rand) {
    for (int x=0;x<10;x++) {
        fmt::println("rand: {}", rand_int(100));
    }
}
