#include <functional>
#include <cassert>
#include <vector>
#include <thread>
#include <fmt/core.h>

std::vector<std::thread> tasks{};

void test_func(const std::function<void()> &foo) {
    assert(foo != nullptr);
    tasks.emplace_back([=]() {
        foo();
        fmt::print("end ...\n");
    });
}

int main() {
    using namespace std::chrono_literals;
    for (int x = 0; x < 10; x++) {
        auto num = x;
        test_func([=]() {
            std::this_thread::sleep_for(10ms);
            fmt::print("num: {}\n", num);
        });
    }
    for (auto &item: tasks) {
        if (item.joinable()) {
            item.join();
        }
    }
}