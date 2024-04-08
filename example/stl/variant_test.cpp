#include "gtest/gtest.h"
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

TEST(variant, multi_types) {
    std::variant<int, double, std::string> x, y, z;
    x = 1;
    y = 1.1;
    z = "111";

    SPDLOG_INFO("x - {}", x.index());
    SPDLOG_INFO("y - {}", y.index());
    SPDLOG_INFO("z - {}", z.index());

    SPDLOG_INFO("x - {}", std::get<int>(x));
}

TEST(variant, single_init) {
    std::variant<std::string, std::string> union_vars{
        std::in_place_index<1>,
        "1",
    };
    if (auto str = std::get_if<0>(&union_vars); str) {
        SPDLOG_INFO("index(0)={}", *str);
    }
    if (auto str = std::get_if<1>(&union_vars); str) {
        SPDLOG_INFO("index(1)={}", *str);
    }
}

TEST(variant, multi_init) {
    std::variant<std::string, std::string> union_vars{
        std::in_place_index<1>,
        "11",
    };

    if (auto str = std::get_if<0>(&union_vars); str) {
        SPDLOG_INFO("before index(0)={}", *str);
    }
    if (auto str = std::get_if<1>(&union_vars); str) {
        SPDLOG_INFO("before index(1)={}", *str);
    }

    union_vars.emplace<0, std::string>("00");

    if (auto str = std::get_if<0>(&union_vars); str) {
        SPDLOG_INFO("after index(0)={}", *str);
    }
    if (auto str = std::get_if<1>(&union_vars); str) {
        SPDLOG_INFO("after index(1)={}", *str);
    }
}