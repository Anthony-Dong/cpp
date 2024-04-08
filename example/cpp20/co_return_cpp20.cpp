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
            return *value_;
        }
        ~promise_type() {
            SPDLOG_INFO("~promise_type");
        }

    private:
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

private:
    std::coroutine_handle<promise_type> handle_;
};

awaitable<int> test(int max) {
    co_return max;
}

awaitable<void> test2() {
}

int main() {
    auto sum = test(10);
    SPDLOG_INFO("sum: {}", sum.value()); // sum: 10 之前就已经析构了~promise_type对象，所以这里或多或少不太安全！
}
