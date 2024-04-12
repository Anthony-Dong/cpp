#include "asio.hpp"
#include "asio/experimental/coro.hpp"
#include "spdlog/spdlog.h"

asio::awaitable<std::string> read_data() {
    const auto executor = co_await asio::this_coro::executor;
    asio::system_timer timer(executor, std::chrono::seconds(1));
    co_await timer.async_wait(asio::use_awaitable);
    co_return "hello world";
}

asio::awaitable<void> run() {
    SPDLOG_INFO("run start");
    auto result = co_await read_data();
    SPDLOG_INFO("run end {}", result);
}

int main(int argc, char* argv[]) {
    asio::io_context ctx;
    auto result =asio::co_spawn(ctx, run(), asio::use_future);
    ctx.run();
    auto data =result.get();
}
