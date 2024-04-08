#include <chrono>

template <typename T>
struct event {
    event() = default;
    virtual ~event() = default;
    [[nodiscard]] virtual bool ready() const = 0;
    [[nodiscard]] virtual T return_value() const = 0;
};

struct timer_event : virtual event<std::chrono::time_point<std::chrono::system_clock>> {
    [[nodiscard]] bool ready() const override {
        return this->point_ >= std::chrono::system_clock::now();
    }

    [[nodiscard]] std::chrono::time_point<std::chrono::system_clock> return_value() const override {
        return this->point_;
    }

    template <typename Rep, typename Period>
    explicit timer_event(std::chrono::duration<Rep, Period> sleep) {
        this->point_ = std::chrono::system_clock::now() + sleep;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> point_;
};

int main() {

    using namespace std::chrono_literals;

    timer_event event_(10s);
    event_.ready();
    event_.return_value();
}