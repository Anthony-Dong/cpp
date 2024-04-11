#include "asio.hpp"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

template <>
struct fmt::formatter<decltype(std::this_thread::get_id())> : fmt::ostream_formatter {};

struct multi_thread {
    explicit multi_thread(const size_t size) : threads_(std::vector<std::thread>(size)) {
    }

    ~multi_thread() {
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void run(const std::function<void()>& foo) {
        for (auto& thread : threads_) {
            thread = std::move(std::thread(foo));
        }
    }

private:
    std::vector<std::thread> threads_;
};

using namespace std::chrono_literals;
TEST(strand, example) {
    asio::io_context ctx{};
    multi_thread multi_thread(4);
    auto wg = asio::make_work_guard(ctx);
    multi_thread.run([&] {
        SPDLOG_INFO("{} {}", std::this_thread::get_id(), "run");
        ctx.run();
    });

    std::this_thread::sleep_for(5ms);

    std::allocator<void> alloc;
    for (int i = 0; i < 10; i++) {
        ctx.get_executor().post(
            [i] {
                SPDLOG_INFO("{}  task id: {} run!", std::this_thread::get_id(), i);
            },
            alloc);
    }
    // std::this_thread::sleep_for(5s);
}

TEST(strand, strand_example) {
    asio::io_context ctx{};
    auto wg = asio::make_work_guard(ctx);
    multi_thread(4).run([&]() {
        ctx.run();
    });
    std::allocator<void> alloc;
    asio::io_context::strand strand(ctx);
    for (int i = 0; i < 10; i++) {
        strand.post(
            [i] {
                SPDLOG_INFO("task id: {} run!", i);
            },
            alloc);
    }
    std::this_thread::sleep_for(5s);
}