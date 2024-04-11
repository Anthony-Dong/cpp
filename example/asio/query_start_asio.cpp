#include "asio.hpp"
#include "spdlog/spdlog.h"
#include "fmt/chrono.h"

template <class Rep, class Period>
asio::awaitable<void> do_print(const std::string& task_name, std::chrono::duration<Rep, Period> sleep) {
    // 获取当前coroutine的执行器
    auto executor = co_await asio::this_coro::executor;

    for (;;) {
        // 创建一个timer
        auto steady_timer = asio::steady_timer(executor, sleep);
        // 等待这个timer触发
        co_await steady_timer.async_wait(asio::deferred);
        SPDLOG_INFO("[{}] sleep {}", task_name, sleep);
    }
}

int main() {
    using namespace std::chrono_literals;

    // 1. 初始化io_context, 你可以理解为是一个调度器
    asio::io_context ctx{};

    std::string task1="task1";
    std::string task2="task2";

    // 2. 创建异步任务 绑定 到 io_context
    asio::co_spawn(ctx, do_print(task1, 1s), asio::detached);
    asio::co_spawn(ctx, do_print(task2, 2s), asio::detached);

    // 3. 运行io_context
    ctx.run();
}