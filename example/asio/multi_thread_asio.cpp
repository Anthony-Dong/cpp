#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "spdlog/spdlog.h"

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

asio::awaitable<void> echo(tcp::socket socket) {
    try {
        char data[1024];
        for (;;) {
            const auto size = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
            // SPDLOG_INFO("{} {} - read", thread_id(), socket.remote_endpoint());
            co_await async_write(socket, asio::buffer(data, size), asio::use_awaitable);
            // SPDLOG_INFO("{} {} - write", thread_id(), socket.remote_endpoint());
        }
    } catch (std::exception& e) {
        SPDLOG_INFO("{} {} - echo Exception: {}", thread_id(), socket.remote_endpoint(), e.what());
        socket.close();
    }
}

asio::awaitable<void> handler_listener(tcp::acceptor& listener) {
    for (;;) {
        auto conn = co_await listener.async_accept(asio::use_awaitable);
        SPDLOG_INFO("{} - receive conn {} -> {}", thread_id(), conn.remote_endpoint(), conn.local_endpoint());
        asio::co_spawn(listener.get_executor(), echo(std::move(conn)), asio::detached);
    }
}

struct multi_thread {
    explicit multi_thread(const size_t size) : threads_(std::vector<std::thread>(size)) {
    }

    ~multi_thread() {
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void run(const std::function<void()>& foo) {
        for (auto& thread : threads_) {
            thread = std::move(std::thread(foo));
        }
    }

private:
    std::vector<std::thread> threads_;
};


int main() {
    asio::io_context ctx;
    tcp::acceptor listener(ctx, tcp::endpoint(tcp::v4(), 8080));
    asio::co_spawn(ctx, handler_listener(listener), asio::detached);
    multi_thread multi_threads(4);
    multi_threads.run([&] {
        ctx.run();
    });
}