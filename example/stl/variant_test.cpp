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

TEST(variant, multi_init_get) {
    std::string data = "111";
    std::variant<std::string, std::reference_wrapper<std::string>> union_vars{
        std::in_place_index<1>,
        data,
    };
    if (union_vars.index() == 1) {
        auto& str = std::get<1>(union_vars).get();
        SPDLOG_INFO("index = 1, value = {}", str);
        str = "222";
    }

    SPDLOG_INFO("data = {}", data);
}

TEST(variant, reference) {
    std::string data = "111";
    std::variant<std::string, std::reference_wrapper<std::string>> union_vars{
        std::in_place_index<1>,
        data,
    };
    auto& str = std::get<1>(union_vars).get();
    str = "222";

    SPDLOG_INFO("data = {}", data);
}

TEST(variant, default_) {
    std::variant<std::string, std::string> union_vars{
        std::in_place_index<1>,
        "111",
    };
    SPDLOG_INFO("before = {}", std::get<1>(union_vars));
    std::get<1>(union_vars) = "222";
    SPDLOG_INFO("after = {}", std::get<1>(union_vars));
}


TEST(variant, const_) {
    const std::variant<std::string, std::string> union_vars{
        std::in_place_index<1>,
        "111",
    };
    SPDLOG_INFO("before = {}", std::get<1>(union_vars));
    // std::get<1>(union_vars) = "222";
}

TEST(variant, structs) {
    std::variant<std::string, std::string> union_vars{};
}