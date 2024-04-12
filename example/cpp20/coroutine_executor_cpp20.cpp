#include "spdlog/spdlog.h"
#include <coroutine>
#include <iostream>
#include <list>

// 事件
template <typename T>
struct event {
    event() = default;
    virtual ~event() = default;
    [[nodiscard]] virtual bool ready() const = 0;
    [[nodiscard]] virtual T return_value() const = 0;
};

// 任务，一个可以被恢复执行的任务
struct task {
    task() = default;
    virtual ~task() = default;
    [[nodiscard]] virtual bool ready() = 0;
    virtual void resume() = 0;
};

// 执行器，执行任务!
struct executor {
    executor() = default;
    ~executor() = default;
    void push(task* task) {
        tasks_.push_back(task);
    }
    void run() {
        std::vector<task*> rm_tasks;
        while (true) {
            if (tasks_.empty()) {
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
            std::this_thread::yield(); // 让出cpu调度防止空转
        }
    }

private:
    std::list<task*> tasks_{};
};

// timer_event
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

struct simple_task final : task {
    bool ready() override {
        return this->is_ready_();
    }
    void resume() override {
        this->resume_();
    }
    simple_task(const std::function<bool()>& is_ready, const std::function<void()>& resume) : is_ready_(is_ready), resume_(resume) {
    }

private:
    std::function<bool()> is_ready_;
    std::function<void()> resume_;
};

//
struct async {
    struct promise_type;
    using coroutine_handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        // 1. 创建coroutine的时候因为它没办法直接调用coroutine的构造函数，因为开放了一个 coroutine::promise_type::get_return_object 函数来创建coroutine对象！
        async get_return_object() {
            SPDLOG_INFO("async::promise_type::get_return_object");
            return async{coroutine_handle::from_promise(*this)};
        }

        // 2. get_return_object 后会调用 initial_suspend 需要返回一个 awaitables 对象
        // 2.1 如果返回 std::suspend_never，表示继续执行，即进入函数体执行
        std::suspend_never initial_suspend() {
            SPDLOG_INFO("async::promise_type::initial_suspend");
            return {};
        }
        // coroutine 结束
        std::suspend_never final_suspend() noexcept {
            SPDLOG_INFO("async::promise_type::final_suspend");
            return {};
        }
        // co_return void 结束
        void return_void() {
            SPDLOG_INFO("async::promise_type::return_void");
        }
        // coroutine 抛出异常
        void unhandled_exception() {
            SPDLOG_INFO("async::promise_type::unhandled_exception");
        }
        // 这个是我们自己实现的函数!
        void resume() {
            SPDLOG_INFO("async::promise_type::resume");
            const auto handle = coroutine_handle::from_promise(*this);
            if (!handle) {
                return;
            }
            handle();
        }
    };

    template <typename T>
    struct event_awaiter {
        [[nodiscard]] auto await_ready() const noexcept {
            SPDLOG_INFO("async::event_awaiter::await_ready()");
            // 在async函数中
            // 1. 事件是否准备好
            // 2. 没有准备好执行 await_suspend
            return this->event_.ready();
        }

        [[nodiscard]] auto await_suspend(std::coroutine_handle<promise_type> handler) noexcept {
            SPDLOG_INFO("async::event_awaiter::await_suspend()");
            // 3. 没有准备好，放入 executor_ 的队列中，并且让出当前执行函数，等待调度器恢复
            this->executor_.push(new simple_task{[this]() -> bool {
                                                     return this->event_.ready();
                                                 },
                                                 [handler]() -> void {
                                                     handler.promise().resume();
                                                 }});
        }
        [[nodiscard]] auto await_resume() const noexcept {
            SPDLOG_INFO("async::event_awaiter::await_resume()");
            // 执行成功，获取返回值
            return this->event_.return_value();
        }

        event_awaiter(event<T>& event, executor& executor) : event_(event), executor_(executor) {
        }

    private:
        event<T>& event_;
        executor& executor_;
    };

    explicit async(const coroutine_handle& handle) : handle_(handle) {
    }

private:
    coroutine_handle handle_{};
};

template <typename Rep, typename Period>
async sleep(executor& executor, const std::string& name, std::chrono::duration<Rep, Period> sleep_time) {
    timer_event event(sleep_time);
    SPDLOG_INFO("[{}] start event", name);
    co_await async::event_awaiter(event, executor);
    SPDLOG_INFO("[{}] done event.", name);
}


int main() {
    using namespace std::chrono_literals;
    executor executor{};
    sleep(executor, "coroutine-1", 1s); // 会创建一个 coroutine ！
    sleep(executor, "coroutine-2", 2s);
    executor.run();
}