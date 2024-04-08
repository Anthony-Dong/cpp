#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread(const std::string& name) {
    // Wait until main() sends data
    std::unique_lock lk(m);
    cv.wait(lk, [] {
        return ready;
    });

    // after the wait, we own the lock.
    std::cout << name << "Worker thread is processing data\n";
    data += " after processing";
    std::cout << name << "Worker thread signals data processing completed\n";
    ready = false;
}

int main() {
    std::thread worker(worker_thread, "work1");
    std::thread worker2(worker_thread, "work2");

    data = "Example data";
    {
        std::lock_guard lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    worker.join();
    worker2.join();
}