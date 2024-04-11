#include "fmt/ostream.h"
#include "spdlog/spdlog.h"
#include <functional>
#include <thread>
#include <vector>

template <>
struct fmt::formatter<decltype(std::this_thread::get_id())> : fmt::ostream_formatter {};

struct multi_thread {
    explicit multi_thread(const size_t size) : threads_(std::vector<std::thread>(size)) {
    }

    ~multi_thread() {
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void run(const std::function<void()>& foo) {
        for (auto& thread : threads_) {
            thread = std::move(std::thread(foo));
        }
    }

private:
    std::vector<std::thread> threads_;
};

int main() {
    multi_thread multi_thread(5);
    multi_thread.run([]() {
        SPDLOG_INFO("{} ... {}", std::this_thread::get_id(), "run");
    });
}