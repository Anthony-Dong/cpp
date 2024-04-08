#include "spdlog/spdlog.h"
#include <coroutine>
#include <iostream>
#include <list>

// 事件（future）
template <typename T>
struct event {
    event() = default;
    virtual ~event() = default;
    [[nodiscard]] virtual bool ready() const = 0;
    [[nodiscard]] virtual T return_value() const = 0;
};

struct timer_event final : virtual event<std::chrono::time_point<std::chrono::system_clock>> {
    [[nodiscard]] bool ready() const override {
        return std::chrono::system_clock::now() >= this->point_;
    }
    [[nodiscard]] std::chrono::time_point<std::chrono::system_clock> return_value() const override {
        return this->point_;
    }
    template <typename Rep, typename Period>
    void reset(const std::chrono::duration<Rep, Period>& sleep) {
        this->point_ = std::chrono::system_clock::now() + sleep;
    }
    template <typename Rep, typename Period>
    explicit timer_event(const std::chrono::duration<Rep, Period>& sleep) : point_(std::chrono::system_clock::now() + sleep) {
    }

private:
    std::chrono::time_point<std::chrono::system_clock> point_;
};

// 执行器
template <typename Task>
struct executor {
    executor() = default;
    ~executor() {
        tasks_.clear();
    };
    void push(Task task) {
        tasks_.push_back(task);
    }
    template <typename Rep, typename Period>
    void run(std::chrono::duration<Rep, Period> timeout) {
        auto end = std::chrono::system_clock::now() + timeout;
        std::vector<Task> rm_tasks;
        while (true) {
            if (std::chrono::system_clock::now() > end) {
                SPDLOG_INFO("timeout rm_tasks: {}, tasks_: {}", rm_tasks.size(), tasks_.size());
                return;
            }
            if (tasks_.empty()) {
                SPDLOG_INFO("done rm_tasks: {}, tasks_: {}", rm_tasks.size(), tasks_.size());
                return;
            }
            for (auto const& this_task : tasks_) {
                if (this_task->ready()) {
                    this_task->resume();
                    rm_tasks.push_back(this_task);
                }
            }
            for (auto const& rm_task : rm_tasks) {
                tasks_.remove(rm_task);
                delete rm_task;
            }
            rm_tasks.clear();
        }
    }

private:
    std::list<Task> tasks_;
};

struct task {
    bool ready() {
        return this->is_ready_();
    }
    void resume() {
        this->resume_();
    }
    task(const std::function<bool()>& is_ready, const std::function<void()>& resume) : is_ready_(is_ready), resume_(resume) {
    }

private:
    std::function<bool()> is_ready_;
    std::function<void()> resume_;
};

struct awaitable {
    struct promise_type;
    using coroutine_handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        // 1. 创建 coroutine 会创建一个 promise (promise_type), 然后会调用 promise_type->get_return_object()
        awaitable get_return_object() {
            SPDLOG_INFO("task::get_return_object");
            return awaitable{coroutine_handle::from_promise(*this)};
        }

        // 2. 初始化成功后需要返回一个 awaiter 对象
        // 2.1 如果返回 std::suspend_never，表示继续执行，即进入函数体执行
        static std::suspend_never initial_suspend() {
            SPDLOG_INFO("task::initial_suspend");
            return {};
        }
        std::suspend_never final_suspend() noexcept {
            SPDLOG_INFO("task::final_suspend");
            return {};
        }
        void return_void() {
            SPDLOG_INFO("task::return_void");
        }
        void unhandled_exception() {
            SPDLOG_INFO("task::unhandled_exception");
        }
        void resume() {
            auto handle = coroutine_handle::from_promise(*this);
            if (handle.done()) {
                return;
            }
            handle.resume();
        }
    };

    template <typename T>
    struct event_awaiter {

        [[nodiscard]] auto await_ready() const noexcept {
            // 在async函数中
            // 1. 事件是否准备好
            // 2. 没有准备好执行 await_suspend
            return this->event_.ready();
        }

        [[nodiscard]] auto await_suspend(std::coroutine_handle<promise_type> handler) noexcept {
            SPDLOG_INFO("await_suspend() start");
            // 3. 没有准备好，放入 executor_ 的队列中，并且让出当前执行栈
            this->executor_.push(new task{[this]() -> bool {
                                              return this->event_.ready();
                                          },
                                          [handler]() -> void {
                                              handler.promise().resume();
                                          }});
            SPDLOG_INFO("await_suspend() end");
        }
        [[nodiscard]] auto await_resume() const noexcept {
            // 执行成功，获取返回值
            return this->event_.return_value();
        }

        event_awaiter(event<T>& event, executor<task*>& executor) : event_(event), executor_(executor) {
        }

    private:
        event<T>& event_;
        executor<task*>& executor_;
    };

    explicit awaitable(const coroutine_handle& handle) : handle_(handle) {
    }

private:
    coroutine_handle handle_;
};

awaitable sleep(executor<task*>& executor, const std::string& name, int64_t sleep_time) {
    std::chrono::milliseconds duration(sleep_time);
    timer_event event(duration);
    SPDLOG_INFO("[{}] start event", name);
    co_await awaitable::event_awaiter(event, executor);
    SPDLOG_INFO("[{}] done event.", name);
}

void test() {
    executor<task*> executor{};
    sleep(executor, "t1", 100);
    sleep(executor, "t2", 200);
    using namespace std::chrono_literals;
    executor.run(1s);
}

int main() {
    test();
    SPDLOG_INFO("main done");
}