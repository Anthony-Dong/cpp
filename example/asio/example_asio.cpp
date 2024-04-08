#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/signal_set.hpp"
#include "asio/write.hpp"
#include <cstdio>
#include <iostream>

using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;
namespace this_coro = asio::this_coro;

awaitable<void> echo(tcp::socket socket) {
    try {
        char data[1024];
        for (;;) {
            const auto size = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
            co_await async_write(socket, asio::buffer(data, size), use_awaitable);
        }
    } catch (std::exception& e) {
        std::printf("echo Exception: %s\n", e.what());
    }
}

awaitable<void> listener() {
    std::cout << "listener co_await executor start\n";
    const auto executor = co_await this_coro::executor;
    std::cout << "listener co_await executor end\n";
    tcp::acceptor acceptor(executor, asio::ip::basic_endpoint(tcp::v4(), 10086));
    std::cout << "listener acceptor success end\n";
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, echo(std::move(socket)), detached);
    }
}

int main() {
    try {
        asio::io_context io_context(1);

        asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_context.stop();
        });
        std::cout << "main co_spawn start\n";
        co_spawn(io_context, listener(), detached);
        std::cout << "main co_spawn end\n";
        io_context.run();
    } catch (std::exception& e) {
        std::printf("Exception: %s\n", e.what());
    }
}