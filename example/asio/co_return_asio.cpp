#include "asio.hpp"
#include "asio/experimental/coro.hpp"
#include "spdlog/spdlog.h"



asio::awaitable<std::string> thorw_error()  {
    throw std::error_code(1, asio::system_category());
}

asio::awaitable<std::string> read_data() {
    try {
        co_await thorw_error();
    } catch (std::error_code& err) {
        SPDLOG_INFO("err: {}",err.message());
    }
    SPDLOG_INFO("hello workd");
}

asio::awaitable<void> run() {
    SPDLOG_INFO("run start");
    auto result = co_await read_data();
    SPDLOG_INFO("run end {}", result);
}

int main(int argc, char* argv[]) {
    asio::io_context ctx;
    asio::co_spawn(ctx, run(), asio::detached);
    ctx.run();
}
