#pragma once

#include "spdlog/spdlog.h"
#include <coroutine>
#include <utility>

template <typename T>
struct awaitable {

    struct promise_type {
        using coroutine_handle = std::coroutine_handle<promise_type>;

        coroutine_handle get_coroutine_handle() {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }

        // 1. 初始化一个 promise (当执行一个 async 函数时)
        awaitable get_return_object() {
            SPDLOG_INFO("awaitable::get_return_object");
            return awaitable{get_coroutine_handle()};
        }

        // 2. 初始化成功后需要返回一个 awaiter 对象
        // 2.1 如果返回 std::suspend_never，表示继续执行，即进入函数体执行
        static std::suspend_never initial_suspend() {
            SPDLOG_INFO("awaitable::initial_suspend");
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            SPDLOG_INFO("awaitable::final_suspend");
            return {};
        }
        //        void return_void() {
        //            SPDLOG_INFO("awaitable::return_void");
        //        }
        void unhandled_exception() {
            SPDLOG_INFO("awaitable::unhandled_exception");
        }

        std::suspend_always yield_value(T& t) {
            SPDLOG_INFO("awaitable::yield_value(&{})", t);
            this->value_ = std::addressof(t);
            return {};
        }

        std::suspend_always yield_value(T&& t) {
            SPDLOG_INFO("awaitable::yield_value(&&{})", t);
            this->value_ = std::addressof(t);
            return {};
        }

        void return_value(T&& t) {
            SPDLOG_INFO("awaitable::return_value(&&{})", t);
            this->value_ = std::addressof(t);
        }

        T& value() {
            SPDLOG_INFO("awaitable::value(start={})", *value_);
            auto handle = get_coroutine_handle();
            if (handle) {
                handle.resume();
            }
            SPDLOG_INFO("awaitable::value(end={})", *value_);
            return *value_;
        }

        ~promise_type() {
            SPDLOG_INFO("~promise_type");
        }

    private:
        bool done;
        std::add_pointer_t<T> value_;
    };

public:
    explicit awaitable(const std::coroutine_handle<promise_type>& handle) : handle_(handle) {
    }

    T value() {
        return this->handle_.promise().value();
    }
    ~awaitable() {
        SPDLOG_INFO("~awaitable");
    }

    //    ~awaitable(){
    //        if (this->handle_){
    //            this->handle_.destroy();
    //        }
    //    }

private:
    std::coroutine_handle<promise_type> handle_;
};

awaitable<int> test(int max) {
    int sum = 0;
    co_yield sum; // 等价于 co_await promise.yield_value(sum)

    for (int x = 0; x < max; x++) {
        sum += x;
        co_yield sum; // 等价于 co_await promise.yield_value(sum)
    }
    co_return 10086;
}

int main() {
    auto sum = test(10);
    for (int x = 0; x < 10; x++) {
        SPDLOG_INFO("sum({})={}", x, sum.value());
    }
}
