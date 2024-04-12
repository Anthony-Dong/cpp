#include "asio.hpp"
#include "spdlog/spdlog.h"

asio::awaitable<void> sleep() {
    using namespace std::chrono_literals;
    SPDLOG_INFO("steady_timer start");
    asio::steady_timer timer(co_await asio::this_coro::executor, 1s);
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("steady_timer tigger");
}

int main() {
    asio::io_context ctx;
    asio::co_spawn(ctx, sleep(), asio::detached); // 把 sleep这个coroutine(任务) 交给 ctx 去调度
    ctx.run();
}


void test_async() {
    asio::io_context ctx;
    using namespace std::chrono_literals;
    asio::steady_timer timer(ctx, 1s);
    SPDLOG_INFO("steady_timer start");

    timer.async_wait([](auto) {
        SPDLOG_INFO("steady_timer tigger");
    });

    SPDLOG_INFO("steady_timer end");
    ctx.run();
}