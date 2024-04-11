#include "spdlog/spdlog.h"
#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>

struct task {
    struct promise_type {
        task get_return_object() {
            auto handler = std::coroutine_handle<promise_type>::from_promise(*this);
            spdlog::info("task::get_return_object");
            return task{handler};
        }

        std::suspend_never initial_suspend() {
            spdlog::info("task::initial_suspend");
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            spdlog::info("task::final_suspend");
            return {};
        }

        void return_void() {
            spdlog::info("task::return_void");
        }

        void unhandled_exception() {
            spdlog::info("task::unhandled_exception");
        }
    };

    explicit task(const std::coroutine_handle<promise_type>& handle) : handle_(handle) {
    }

    void resume() {
        if (handle_.done()) {
            return;
        }
        handle_.resume();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

struct suspend_always {
    bool await_ready() const noexcept {
        spdlog::info("suspend_always::await_ready");
        return false;
    }

    void await_suspend(std::coroutine_handle<task::promise_type> handler) const noexcept {
        spdlog::info("suspend_always::await_suspend");
    }

    void await_resume() const noexcept {
        spdlog::info("suspend_always::await_resume");
    }
};

task test() {
    spdlog::info("start test");
    co_await suspend_always();
    spdlog::info("end test");
}

int main() {
    auto task = test();
    task.resume();
}