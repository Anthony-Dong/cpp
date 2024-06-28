#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "spdlog/spdlog.h"
#include <exception>
#include <stdexcept>
#include <string_view>

using namespace asio::ip;

std::string thread_id() {
    return std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
}

template <typename T>
struct fmt::formatter<asio::ip::basic_endpoint<T>> : fmt::formatter<std::string_view> {
    auto format(const asio::ip::basic_endpoint<T>& endpoint, fmt::format_context& ctx) {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

asio::awaitable<void> echo(tcp::socket socket) {
    try {
        char data[1024];
        for (;;) {
            const auto size = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
            SPDLOG_INFO("{} {} - read", thread_id(), socket.remote_endpoint());
            if (size >= 5 && std::string_view(data, 5) == "close") {
                throw std::runtime_error("close conn");
            }
            co_await socket.async_send(asio::buffer(data, size), asio::use_awaitable);
            SPDLOG_INFO("{} {} - write", thread_id(), socket.remote_endpoint());
        }
    } catch (std::exception& e) {
        SPDLOG_INFO("{} {} - echo Exception: {}", thread_id(), socket.remote_endpoint(), e.what());
        socket.close();
        throw e;
    }
}
asio::awaitable<void> handler_listener(tcp::acceptor& listener) {
    try {
        for (;;) {
            auto conn = co_await listener.async_accept(asio::use_awaitable);
            SPDLOG_INFO("{} - receive conn {} -> {}", thread_id(), conn.remote_endpoint(), conn.local_endpoint());
            asio::co_spawn(listener.get_executor(), echo(std::move(conn)), asio::detached);
        }
    } catch (std::exception& e) {
        throw e; // 这里抓不到异常哇
    }
}

int main() {
    asio::io_context ctx;
    std::allocator<void> alloc;
    tcp::acceptor listener(ctx, tcp::endpoint(tcp::v4(), 8080));
    try {
        asio::co_spawn(ctx, handler_listener(listener), asio::detached);
    } catch (...) {
        throw;
    }
    ctx.run();
}