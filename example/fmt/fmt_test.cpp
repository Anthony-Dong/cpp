#include <fmt/core.h>
#include <fmt/ostream.h>

#include <gtest/gtest.h>
#include <thread>

template <>
struct fmt::formatter<decltype(std::this_thread::get_id())> : fmt::ostream_formatter {};

struct MyStruct {
    explicit MyStruct(std::string name) : name(std::move(name)) {
    }

private:
    friend fmt::formatter<MyStruct>;
    std::string name;
};

enum class film {
    house_of_cards = 1,
    american_beauty = 2,
    se7en = 7
};

auto format_as(const film& f) {
    return fmt::underlying(f);
}

TEST(fmt, fmt_ostream_formatter) {
    fmt::println("thread-id: {}", std::this_thread::get_id());
}

template <>
struct fmt::formatter<MyStruct> : fmt::formatter<std::string> {
    auto format(const MyStruct& value, auto& ctx) const {
        return fmt::format_to(ctx.out(), "MyStruct(name={})", value.name);
    }
};

TEST(fmt, custom_formatter) {
    MyStruct data("xiaoli");
    fmt::println("MyStruct: {}", data);
}

TEST(fmt, enum_formatter) {
    fmt::println("film: {}", film::american_beauty);
}

TEST(fmt, chrono) {
    using namespace std::chrono_literals;
    fmt::println("cost: {}", 10s);
}
