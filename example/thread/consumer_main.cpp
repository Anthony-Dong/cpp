#include "spdlog/spdlog.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
struct Queue {

    void push(T t) {
        std::lock_guard lock(mutex_);
        queue_.push(t);
        cond_.notify_one();
    };

    T pop() {
        std::unique_lock lock(mutex_);
        cond_.wait(lock, [this]() {
            return this->queue_.size() > 0;
        });
        T value = queue_.front();
        queue_.pop();
        return value;
    }

private:
    std::queue<T> queue_{};
    std::mutex mutex_{};
    std::condition_variable cond_{};
};

void consumer(Queue<int>& queue) {
    while (1) {
        auto x = queue.pop();
        SPDLOG_INFO("num: {}", x);
    }
}

void producer(Queue<int>& queue) {
    int x = 0;
    while (1) {
        queue.push(x);
        x++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    Queue<int> queue{};

    std::thread t1(consumer, std::ref(queue));
    std::thread t2(producer, std::ref(queue));
    std::thread t3(consumer, std::ref(queue));
    t1.join();
    t2.join();
    t3.join();
}