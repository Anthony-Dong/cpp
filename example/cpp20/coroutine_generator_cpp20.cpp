#include <coroutine>
#include <iostream>
#include <iterator>
#include <utility>

namespace libco {

struct GeneratorEnd {};

template <typename HandleType, typename ValueType>
struct GeneratorIter {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<ValueType>;
    using reference = std::conditional_t<std::is_reference_v<ValueType>, ValueType, ValueType&>;
    using pointer = std::add_pointer_t<ValueType>;

    bool operator!=(const GeneratorEnd&) {
        return !h.done();
    }
    void operator++() {
        h.resume();
    }
    value_type& operator*() {
        return h.promise().value();
    }

    HandleType h;
};

template <typename T>
class SimpleGenerator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    struct promise_type {
        using pointer = std::add_pointer_t<T>;
        pointer value_;

        auto co() {
            return handle_type::from_promise(*this);
        }
        auto get_return_object() {
            return SimpleGenerator(co());
        }
        std::suspend_always initial_suspend() {
            return {};
        }
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        void unhandled_exception() {
            throw;
        }

        std::suspend_always yield_value(T& value) {
            value_ = std::addressof(value);
            return {};
        }
        std::suspend_always yield_value(T&& value) {
            value_ = std::addressof(value);
            return {};
        }
        void return_void() {
        }

        T& value() {
            return *value_;
        }
    };

    explicit SimpleGenerator(handle_type h) : h_(h) {
    }
    ~SimpleGenerator() {
        if (h_) {
            h_.destroy();
        }
    }

    auto end() const {
        return GeneratorEnd{};
    }
    auto begin() {
        auto it = GeneratorIter<handle_type, T>{h_};
        if (!begin_) {
            ++it;
            begin_ = true;
        }
        return it;
    }

private:
    handle_type h_;
    bool begin_ = false;
};
} // namespace libco

libco::SimpleGenerator<int> Fib(int n) {
    if (n == 0 || n == 1) {
        co_return;
    }
    int a = 0, b = 1;
    co_yield a;
    co_yield b;

    for (int i = 2; i <= n; ++i) {
        co_yield a + b;

        b = a + b;
        a = b - a;
    }
    co_return;
}

int main() {
    auto g = Fib(10);
    int i = 0;
    for (auto begin = g.begin(); begin != g.end(); ++begin) {
        std::cout << "Fib (" << i++ << ")=" << *g.begin() << "\n";
    }
}
