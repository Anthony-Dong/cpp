#include <condition_variable>
#include <thread>
#include <queue>
#include <array>
#include <iostream>
#include <functional>
#include <atomic>

template <size_t Size>
struct ThreadPool {
    struct Executor {
        enum Status {
            Init,
            Running,
            Wait,
        };
        void Run(ThreadPool* pool) {
            pool_ = pool;
            status_ = Running;
            thread_ = std::thread([&]() {
                this->run();
            });
        }
        ~Executor() {
            if (thread_.joinable()) {
                thread_.join();
            }
        }
        Status GetStatus() {
            return status_;
        }

    private:
        void run() {
            while (true) {
                std::unique_lock<std::mutex> lock(pool_->mutex_);
                pool_->condition_.wait(lock, [this] {
                    return pool_->tasks_.size() > 0 || pool_->is_close;
                });
                if (pool_->is_close) {
                    lock.unlock();
                    return;
                }
                auto& task = pool_->tasks_.front();
                pool_->tasks_.pop();
                lock.unlock();
                task();
            }
        }

    private:
        std::thread thread_{};
        Status status_{};
        ThreadPool* pool_{};
    };
    ~ThreadPool() {
        std::lock_guard lock(mutex_);
        is_close = true;
        condition_.notify_all();
    };
    void Run(const std::function<void()>& runnable) {
        {
            std::lock_guard lock(mutex_);
            tasks_.push(runnable);
            for (auto& executor : executors_) {
                if (executor.GetStatus() == Executor::Status::Init) {
                    executor.Run(this);
                    break;
                }
            }
        }
        condition_.notify_one();
    }

private:
    friend Executor;
    std::queue<std::function<void()>> tasks_{};
    std::condition_variable condition_{};
    std::mutex mutex_{};
    bool is_close{};

    std::array<Executor, Size> executors_{};
};

class WaitGroup {
public:
    void Add(int incr = 1) {
        counter += incr;
    }
    void Done() {
        if (--counter <= 0)
            cond.notify_all();
    }
    void Wait() {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [&] {
            return counter <= 0;
        });
    }

private:
    std::mutex mutex{};
    std::atomic<int> counter{};
    std::condition_variable cond{};
};

int main() {
    int sum = 0;
    WaitGroup wg;
    ThreadPool<10> pool{};
    std::mutex mutex{};
    for (int x = 0; x < 10000; x++) {
        wg.Add(1);
        pool.Run([&mutex, &sum, &wg]() {
            {
                std::lock_guard lock(mutex);
                ++sum;
            }
            wg.Done();
        });
    }
    wg.Wait();
    std::cout << "sum: " << sum << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
}