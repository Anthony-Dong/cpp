#include <coroutine>
#include <iostream>

/*
 * 支持 return_void & return_type
 */

struct promise_type_base {
    std::suspend_never initial_suspend() {
        return {};
    }
    std::suspend_never final_suspend() noexcept {
        return {};
    }
    void unhandled_exception() {
    }
};

struct promise_type_base_return_void : promise_type_base {
    void return_void() {
    }
};

template <typename T>
struct promise_type_base_return_t : promise_type_base {
    void return_value(T&& t) {
        this->t = std::forward<T>(t);
    }
    T& value() {
        return t;
    }

private:
    T t;
};

template <typename T>
struct awaitable {
    struct promise_type : promise_type_base_return_t<T> {
        auto get_return_object() {
            return awaitable{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
    };

    [[nodiscard]] constexpr bool await_ready() const noexcept {
        return true;
    }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {
    }

    [[nodiscard]] T& await_resume() const noexcept {
        return this->handle_.promise().value();
    }

    explicit awaitable(const std::coroutine_handle<promise_type>& handle) : handle_(handle) {
    }

private:
    std::coroutine_handle<promise_type> handle_{};
};

struct awaitable_base_void {
    void await_resume() const noexcept {
    }
};

template <>
struct awaitable<void> : awaitable_base_void {
    struct promise_type : promise_type_base_return_void {
        auto get_return_object() {
            return awaitable{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
    };
    explicit awaitable(const std::coroutine_handle<promise_type>& handle) : handle_(handle) {
    }

private:
    std::coroutine_handle<promise_type> handle_{};
};

awaitable<int> test(int max) {
    co_return max;
}

awaitable<void> test2() {
    const auto max = co_await test(100);
    std::cout << "max: " << max << "\n";
}

int main() {
    test2();
}
