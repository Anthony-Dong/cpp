#include "fmt/core.h"

struct Test {
    int age;
    std::string name;
};

struct ParseNop {};

template <>
struct fmt::formatter<ParseNop> {
    template <typename Context>
    constexpr auto parse(Context& ctx) {
        return ctx.begin();
    }
};

template <>
struct fmt::formatter<Test> : fmt::formatter<ParseNop> {
    template <typename Context>
    auto format(const Test& data, Context& ctx) {
        return fmt::format_to(ctx.out(), "name: {}. age: {}", data.name, data.age);
    }
};

int main() {
    Test data{42, "Test"};
    fmt::println("test: {}", data);
}

// [ 1 ,2 ,3 ,]
// [3 ,4 ,5 ,]
// double_end_queue [ 1, 3]
// double_end_queue [ 2, 3]
// double_end_stack-top [ 3]
// double_end_stack-top [ 2]
// hash_map: [2: 2]
// hash_map: [3: 3]
// hash_map: [1: 1]
// tree_map: [1: 1]
// tree_map: [2: 2]
// tree_map: [3: 3]