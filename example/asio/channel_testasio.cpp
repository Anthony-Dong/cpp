#include "asio.hpp"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <asio/experimental/awaitable_operators.hpp>
#include <asio/experimental/channel.hpp>

using int_channel = asio::experimental::channel<void(asio::error_code, int)>;

std::string thread_id() {
    return std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
}

asio::awaitable<void> sync_producer(int_channel& channel) {
    for (int x = 0; x < 10; x++) {
        co_await channel.async_send(asio::error_code{}, x, asio::use_awaitable);
        SPDLOG_INFO("async_send {} {}", x, thread_id());
    }
}

asio::awaitable<void> sync_consumer(int_channel& channel) {
    for (int x = 0; x < 10; x++) {
        auto num = co_await channel.async_receive(asio::use_awaitable);
        SPDLOG_INFO("sync_consumer {}", num, thread_id());
    }
}

TEST(asio_channel, none_bufferd) {
    asio::io_context io_context;
    int_channel channel(io_context);
    asio::co_spawn(io_context, sync_producer(channel), asio::detached);
    asio::co_spawn(io_context, sync_consumer(channel), asio::detached);
    io_context.run();
}

TEST(asio_channel, bufferd) {
    asio::io_context io_context;
    int_channel channel(io_context, 10);
    asio::co_spawn(io_context, sync_producer(channel), asio::detached);
    io_context.run();
}

template <typename T>
struct fmt::formatter<asio::ip::basic_endpoint<T>> : fmt::formatter<std::string_view> {
    auto format(const asio::ip::basic_endpoint<T>& endpoint, fmt::format_context& ctx) {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

enum class film {
    house_of_cards = 1,
    american_beauty = 2,
    se7en = 7
};
auto format_as(const film& f) {
    return fmt::underlying(f);
}

TEST(asio_tcp, fmt) {
    SPDLOG_INFO("{}", film::se7en);
}

TEST(asio_tcp, endpoint) {
    using namespace asio::ip;
    basic_endpoint endpoint(tcp::v4(), 10086);
    SPDLOG_INFO("{}", endpoint);
}