#include "asio.hpp"
#include "asio/experimental/channel.hpp"
#include "spdlog/spdlog.h"

// 注意 channel 必需第一个参数是 asio::error_code，其他是自己需要传递的参数，可以多个！
// channel是绑定了io_context不能跨io_context调度！
using int_channel = asio::experimental::channel<void(asio::error_code, int)>;

asio::awaitable<void> produce(int_channel& channel) {
    for (int x = 0; x < 10; x++) {
        co_await channel.async_send(asio::error_code{}, x + 1, asio::use_awaitable);
    }
    // 用完后记得close
    channel.close();
}

asio::awaitable<void> consume(int_channel& channel) {
    for (;;) {
        auto [errcode, num] = co_await channel.async_receive(asio::as_tuple(asio::use_awaitable));
        if (errcode) {
            if (errcode == asio::experimental::channel_errc::channel_closed) {
                SPDLOG_INFO("channel_closed");
                co_return;
            }
            SPDLOG_INFO("system error code: {}, message: {}", errcode.value(), errcode.message());
            co_return;
        }
        SPDLOG_INFO("receive: {}", num);
    }
}

asio::awaitable<void> consume_try_catch(int_channel& channel) {
    asio::steady_timer timer(channel.get_executor(), std::chrono::seconds(1));
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("start receive");
    for (;;) {
        try {
            auto num = co_await channel.async_receive(asio::use_awaitable);
            SPDLOG_INFO("receive: {}", num);
        } catch (const std::exception& err) {
            // 也可以 try catch抓
            SPDLOG_INFO("error: {}", err.what());
            co_return;
        }
    }
}

int main(int argc, char* argv[]) {
    asio::io_context ctx;
    int_channel channel(ctx);
    asio::co_spawn(ctx, produce(channel), asio::detached);
    asio::co_spawn(ctx, consume(channel), asio::detached);
    ctx.run();
}
