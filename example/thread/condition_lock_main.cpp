#include <condition_variable>
#include <thread>
#include <iostream>

std::condition_variable cond;
std::mutex mutex;
bool ready = false;

void task1() {
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [] {
        return ready;
    });
}

void task2() {
    ready = true;
    cond.notify_one();
}

void task(int num) {
    std::thread t1(task1);
    std::thread t2(task2);
    t1.join();
    t2.join();

    std::string output = "done: ";
    output = output + std::to_string(num);
    std::cout << (output + "\n");
}

int main() {
    for (int x = 0; x < 5000; x++) {
        task(x);
    }
}