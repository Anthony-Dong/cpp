#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"

template <>
struct fmt::formatter<decltype(std::this_thread::get_id())> : fmt::ostream_formatter {};

std::string thread_id() {
    return fmt::to_string(std::this_thread::get_id());
}

template <typename T>
struct fmt::formatter<asio::ip::basic_endpoint<T>> : fmt::formatter<std::string_view> {
    auto format(const asio::ip::basic_endpoint<T>& endpoint, fmt::format_context& ctx) {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

struct worker_context {
    worker_context() = default;

    void run() {
        thread_ = std::move(std::thread([this] {
            io_ctx.run();
        }));
    }

    ~worker_context() {
        io_ctx.stop();
        if (this->thread_.joinable()) {
            thread_.join();
        }
    }

    asio::awaitable<void> echo(asio::ip::tcp::socket socket) {
        SPDLOG_INFO("{} - handle conn {}", thread_id(), socket.remote_endpoint());
        try {
            char data[1024];
            for (;;) {
                const auto size = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
                //    SPDLOG_INFO("{} {} - read", thread_id(), socket.remote_endpoint());
                co_await async_write(socket, asio::buffer(data, size), asio::use_awaitable);
                //  SPDLOG_INFO("{} {} - write", thread_id(), socket.remote_endpoint());
            }
        } catch (std::exception& e) {
            SPDLOG_INFO("{} {} - echo Exception: {}", thread_id(), socket.remote_endpoint(), e.what());
            socket.close();
        }
    }
    asio::io_context& get_context() {
        return io_ctx;
    }

private:
    asio::io_context io_ctx{};
    std::thread thread_{};
};

struct main_context {
    using work_guard_type = decltype(asio::make_work_guard(std::declval<worker_context>().get_context()));
    explicit main_context(size_t worker_size) : worker_contexts_(std::vector<worker_context>(worker_size)){};

    void run() {
        asio::ip::tcp::acceptor listener(io_ctx_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));
        asio::co_spawn(io_ctx_, handler_listener(listener), asio::detached);

        std::vector<work_guard_type> work_guards{};
        for (auto& worker_context : worker_contexts_) {
            work_guards.push_back(asio::make_work_guard(worker_context.get_context()));
            worker_context.run();
        }
        io_ctx_.run();
    }

    ~main_context() {
        io_ctx_.stop();
    }

    asio::awaitable<void> handler_listener(asio::ip::tcp::acceptor& listener) {
        for (;;) {
            auto conn = co_await listener.async_accept(asio::use_awaitable);
            conn_counter = conn_counter + 1;
            SPDLOG_INFO("{} - receive conn[{}] {} -> {}", thread_id(), conn_counter, conn.remote_endpoint(), conn.local_endpoint());
            auto& worker_context = worker_contexts_[conn_counter % worker_contexts_.size()];
            asio::co_spawn(worker_context.get_context(), worker_context.echo(std::move(conn)), asio::detached);
        }
    }

private:
    asio::io_context io_ctx_{};
    std::vector<worker_context> worker_contexts_{};
    size_t conn_counter{};
};

int main() {
    main_context main_context(4);
    main_context.run();
}