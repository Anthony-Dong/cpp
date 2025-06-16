#include "asio.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "fmt/chrono.h"
#include "spdlog/spdlog.h"

template <class Rep, class Period>
asio::awaitable<std::string> create_task(std::chrono::duration<Rep, Period> spend, std::string task_name) {
    const auto executor = co_await asio::this_coro::executor;
    asio::steady_timer timer(executor, spend);
    SPDLOG_INFO("create_io_task - {} start. spend {}", task_name, spend);
    co_await timer.async_wait(asio::use_awaitable);
    SPDLOG_INFO("create_io_task - {} end.", task_name);
    co_return task_name;
}

asio::awaitable<void> select_one_task() {
    using namespace asio::experimental::awaitable_operators;
    using namespace std::chrono_literals;
    const auto result = co_await (create_task(1s, "task1") || create_task(2s, "task2") || create_task(3s, "task3"));
    if (const auto name = std::get_if<0>(&result); name != nullptr) {
        SPDLOG_INFO("select task0 - {}", *name);
    }
    if (const auto name = std::get_if<1>(&result); name != nullptr) {
        SPDLOG_INFO("select task1 - {}", *name);
    }
}

asio::awaitable<void> select_one_task_2() {
    using namespace asio::experimental::awaitable_operators;
    using namespace std::chrono_literals;
    const auto result = co_await (create_task(3s, "task1") || create_task(2s, "task2") || create_task(3s, "task3"));

    if (result.index() == 0) {
        SPDLOG_INFO("select task-0 - {}", "task1");
    }
    if (result.index() == 1) {
        SPDLOG_INFO("select task-1 - {}", "task2");
    }
    if (result.index() == 2) {
        SPDLOG_INFO("select task-2 - {}", "task3");
    }
}

asio::awaitable<void> wait_group() {
    using namespace asio::experimental::awaitable_operators;
    using namespace std::chrono_literals;
    const auto& [task1, task2] = co_await (create_task(1s, "task1") && create_task(2s, "task2"));
    SPDLOG_INFO("result: {} {}", task1, task2);
}

int main(int argc, char* argv[]) {
    asio::io_context context{};
    asio::co_spawn(context, select_one_task_2(), asio::detached);
    context.run();
}
