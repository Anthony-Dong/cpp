#include "asio.hpp"
#include "spdlog/spdlog.h"
using namespace std::chrono_literals;

asio::awaitable<void> do_sleep(asio::io_context& context) {
    asio::steady_timer timer(context, std::chrono::seconds(1));
    // 创建一个 deferred_async_operation
    SPDLOG_INFO("do_sleep start");
    const auto timer_async_op = timer.async_wait(asio::deferred);

    // 中间可以做一些别的事情！
    SPDLOG_INFO("do_sleep end");

    // 需要执行的时候再执行 deferred_async_operation
    co_await timer_async_op(asio::use_awaitable);
    SPDLOG_INFO("do_sleep end");
}

asio::awaitable<void> do_sleep_use_awaitable(asio::io_context& context) {
    asio::steady_timer timer(context, std::chrono::seconds(1));
    // 如果你需要直接 await 那么你只需要 use_awaitable，不推荐用 deferred
    SPDLOG_INFO("do_sleep_use_awaitable start");
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("do_sleep_use_awaitable end");
}

int main() {
    asio::io_context context{};
    asio::co_spawn(context, do_sleep(context), asio::detached);
    context.run();
}
