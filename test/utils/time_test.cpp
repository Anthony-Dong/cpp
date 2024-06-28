#include "fmt/core.h"
#include "gtest/gtest.h"

struct MyTest {
    int age;
    std::string name;
};

template <>
struct fmt::formatter<std::string> {
    template <typename Context>
    constexpr auto parse(Context& ctx) {
        return ctx.begin();
    }
};

template <>
struct fmt::formatter<MyTest> : fmt::formatter<std::string> {
    template <typename Context>
    auto format(const MyTest& data, Context& ctx) {
        return fmt::format_to(ctx.out(), "name: {}. age: {}", data.name, data.age);
    }
};

TEST(UtilsTest, TimeTest) {
    MyTest data{42, "Test"};
    fmt::println("test: {}", data);
}