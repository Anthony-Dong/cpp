#include <mutex>
#include <thread>
#include <array>
#include <iostream>

int main() {
    std::mutex mutex;
    int sum = 0;

    auto run = [&]() {
        for (int x = 0; x < 10000; x++) {
            {
                std::unique_lock lock(mutex);
                sum++;
//                lock.unlock();
            }
        }
    };

    std::array<std::thread, 10> threads;

    for (auto& item : threads) {
        item = std::thread(run);
    }

    for (auto& item : threads) {
        if (item.joinable()) {
            item.join();
        }
    }

    std::cout << "sum: " << sum;
}