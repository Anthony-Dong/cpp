#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "spdlog/spdlog.h"
#include <asio.hpp>
#include <iostream>

#include "asio/any_completion_handler.hpp"
#include "asio/experimental/awaitable_operators.hpp"

asio::awaitable<int> echo() {
    const auto executor = co_await asio::this_coro::executor;
    asio::steady_timer timer(executor, std::chrono::milliseconds(1000));
    co_await timer.async_wait(asio::use_awaitable);
    spdlog::info("current: {}", absl::FormatTime(absl::Now()));
    co_return 10;
}

int test_co_return() {
    asio::io_context io_context(1);
    asio::co_spawn(io_context, echo(), asio::any_completion_handler<void(std::exception_ptr, int)>([&](std::exception_ptr _, int i) {
                       SPDLOG_INFO("result: {}", i);
                       return;
                   }));
    io_context.run();
    SPDLOG_INFO("{}", "end");
}

asio::awaitable<std::string> task1() {
    asio::steady_timer timer(co_await asio::this_coro::executor, std::chrono::milliseconds(100));
    co_await timer.async_wait(asio::use_awaitable);
    spdlog::info("task1: {}", absl::FormatTime(absl::Now()));
    co_return "task1";
}

asio::awaitable<std::string> task2() {
    asio::steady_timer timer(co_await asio::this_coro::executor, std::chrono::milliseconds(500));
    co_await timer.async_wait(asio::use_awaitable);
    spdlog::info("task2: {}", absl::FormatTime(absl::Now()));
    co_return "task2";
}

asio::awaitable<void> task() {
    using namespace asio::experimental::awaitable_operators;
    auto result = co_await (task1() || task2());
    if (auto name= std::get_if<0>(&result); name){
        SPDLOG_INFO("result: {}", *name);
    }
    if (auto name= std::get_if<1>(&result); name){
        SPDLOG_INFO("result: {}", *name);
    }
}

int test_co_group() {
    asio::io_context io_context(1);
    asio::co_spawn(io_context, task(), asio::detached);
    io_context.run();
}

int main() {
    test_co_group();
}