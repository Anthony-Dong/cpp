#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "spdlog/spdlog.h"

#include <map>

using namespace asio::ip;

std::string thread_id() {
    return std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
}

template <typename T>
struct fmt::formatter<asio::ip::basic_endpoint<T>> : fmt::formatter<std::string_view> {
    auto format(const asio::ip::basic_endpoint<T>& endpoint, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

asio::awaitable<void> echo(std::shared_ptr<tcp::socket> socket) {
    try {
        char data[1024];
        for (;;) {
            const auto size = co_await socket->async_read_some(asio::buffer(data), asio::use_awaitable);
            // SPDLOG_INFO("{} {} - read", thread_id(), socket.remote_endpoint());
            co_await socket->async_send(asio::buffer(data, size), asio::use_awaitable);
            // SPDLOG_INFO("{} {} - write", thread_id(), socket.remote_endpoint());
        }
    } catch (std::exception& e) {
        SPDLOG_INFO("{} {} - echo Exception: {}", thread_id(), socket->remote_endpoint(), e.what());
        socket->close();
        socket.reset();
    }
}

asio::awaitable<void> handler_conn(tcp::socket socket) {
    using namespace asio::experimental::awaitable_operators;
    using namespace std::chrono_literals;

    auto const excutor = socket.get_executor();
    auto const endpoint = socket.remote_endpoint();

    char buffer[1024];
    while (true) {
        asio::steady_timer read_timeout(excutor, 10s);

        try {
            auto result = co_await (socket.async_read_some(asio::buffer(buffer), asio::use_awaitable) || read_timeout.async_wait(asio::use_awaitable));

            if (const auto size = std::get_if<0>(&result); size != nullptr) {
                const auto read_size = *size;
                // 写出数据 (todo add timeout)
                auto write_size = co_await socket.async_send(asio::buffer(buffer, read_size), asio::use_awaitable);
                // SPDLOG_INFO("{} read({}) & write({})", endpoint, read_size, write_size);
            }

            if (const auto timeout = std::get_if<1>(&result); timeout != nullptr) {
                SPDLOG_INFO("{} find err: {}", endpoint, "read timeout");
                socket.close();
                co_return;
            }
        } catch (const asio::system_error& err) {
            SPDLOG_INFO("{} find err: {} {}", endpoint, err.code().value(), err.code().message());
            socket.close();
            co_return;
        } catch (const std::exception& err) {
            SPDLOG_INFO("{} Caught unknown exception {}", endpoint, err.what());
            socket.close();
        }
    }
}

asio::awaitable<void> handler_listener(tcp::acceptor& listener) {
    for (;;) {
        auto conn = co_await listener.async_accept(asio::use_awaitable);
        SPDLOG_INFO("{} - receive conn {} -> {}", thread_id(), conn.remote_endpoint(), conn.local_endpoint());
        asio::co_spawn(listener.get_executor(), echo(std::make_shared<tcp::socket>(std::move(conn))), asio::detached);
    }
}

int main() {
    asio::io_context ctx;
    std::allocator<void> alloc;
    tcp::acceptor listener(ctx, tcp::endpoint(tcp::v4(), 8080));
    asio::co_spawn(ctx, handler_listener(listener), asio::detached);
    ctx.run();
}