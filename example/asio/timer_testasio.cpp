#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

template <typename AsyncFunc>
int fast_run(AsyncFunc foo) {
    try {
        asio::io_context io_context;
        asio::co_spawn(io_context, foo(), asio::detached);
        io_context.run();
    } catch (const std::exception& err) {
        SPDLOG_INFO("find err: {}", err.what());
    }
}

template <typename R, typename P>
asio::awaitable<void> awaiter(std::chrono::duration<R, P> tomeout) {
    using namespace std::chrono_literals;
    asio::steady_timer timer(co_await asio::this_coro::executor);
    timer.expires_after(tomeout);
    co_await timer.async_wait(asio::use_awaitable);
    if (tomeout < 2s) {
        co_return;
    }
    throw std::runtime_error("timeout");
}

asio::awaitable<void> timeout() {
    using namespace std::chrono_literals;
    using namespace asio::experimental::awaitable_operators;
    auto executor = co_await asio::this_coro::executor;
    while (true) {
        SPDLOG_INFO("async_wait start");
        try {
            co_await (asio::steady_timer(executor, 2s).async_wait(asio::use_awaitable) || asio::steady_timer(executor, 1s).async_wait(asio::use_awaitable));
        } catch (const std::exception& err) {
            SPDLOG_INFO("find err: {}", err.what());
            co_return;
        }
        SPDLOG_INFO("async_wait end");
    }
}

TEST(asio_timer, simple) {
    fast_run(timeout);
}