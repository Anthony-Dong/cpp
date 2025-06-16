#include "gtest/gtest.h"
#include "cpp/log/logger.h"

struct MyTest {
    int age;
    std::string name;
};

template <> // 特化模版 fmt::formatter<T>
struct fmt::formatter<MyTest> : fmt::formatter<char> {
    template <typename Context>
    auto format(const MyTest& data, Context& ctx) const {
        return fmt::format_to(ctx.out(), "name: {}. age: {}", data.name, data.age);
    }
};

TEST(LogTest, Print) {
    INFO("hello {}", "world");

    MyTest data{.age = 1, .name = "hello"};
    INFO("MyTest {}", data);

    // cpp::log::Log(cpp::log::Level::Debug, "logger_test.cpp", 7, "", "hello {}", "1111");
}