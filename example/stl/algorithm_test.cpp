//
// Created by bytedance on 2024/4/10.
//
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

TEST(algorithm, range) {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (const auto& item : numbers) {
        SPDLOG_INFO("item: {}", item);
    }
}