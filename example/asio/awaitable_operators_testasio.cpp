#include "asio/awaitable.hpp"
#include "asio/detached.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

asio::awaitable<std::string> task1() {
    asio::steady_timer timer(co_await asio::this_coro::executor, std::chrono::milliseconds(100));
    SPDLOG_INFO("task1 start");
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("task1 end");
    co_return "task1";
}

asio::awaitable<std::string> task2() {
    asio::steady_timer timer(co_await asio::this_coro::executor, std::chrono::milliseconds(500));
    SPDLOG_INFO("task2 start");
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("task2 end");
    co_return "task2";
}

asio::awaitable<void> task() {
    using namespace asio::experimental::awaitable_operators;
    auto result = co_await (task1() || task2());
    if (auto name = std::get_if<0>(&result); name) {
        SPDLOG_INFO("task(1): {}", *name);
    }
    if (auto name = std::get_if<1>(&result); name) {
        SPDLOG_INFO("task(2): {}", *name);
    }
}

TEST(asio, awaitable_operators) {
    asio::io_context io_context;
    asio::co_spawn(io_context, task(), asio::detached);
    io_context.run();
}
