#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fmt/core.h>
#include <thread>
#include <sys/file.h>

#define ERROR_LOG(error) fmt::println(error ": {}", strerror(errno))

/**
➜  data cat flock.log | wc -l
2000
➜  data cat flock.log | grep '18140' | wc -l
1000
➜  data cat flock.log | grep '18133' | wc -l
1000
 */
int main() {
    const char* filename = "/Users/bytedance/data/flock.log";
    auto fd = open(filename, O_WRONLY | O_CREAT | O_APPEND);
    if (fd == -1) {
        std::cout << "open file find err: " << strerror(errno) << "\n";
        return -1;
    }
    fmt::println("open file success: {}", filename);

    auto use_file_lock = false;

    auto start = std::chrono::system_clock::now();

    auto pid = getpid();
    fmt::println("pid: {}", pid);

    auto write_file = [&]() {
        if (use_file_lock) {
            if (auto status = flock(fd, LOCK_EX); status == -1) {
                ERROR_LOG("lock file err");
                return -1;
            }
        }
        auto line = fmt::format("hello world: {}\n", pid);
        if (auto status = write(fd, line.data(), line.size()); status == -1) {
            ERROR_LOG("write file error");
            return -1;
        }
        if (use_file_lock) {
            if (auto status = flock(fd, LOCK_UN); status == -1) {
                ERROR_LOG("unlock file error");
                return -1;
            }
        }
        return 0;
    };
    using namespace std::chrono_literals;
    for (int x = 0; x < 20000; x++) {
        if (auto status = write_file(); status == -1) {
            return status;
        }
        std::this_thread::sleep_for(1ms);
    }
    close(fd);
    auto spend = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    fmt::println("spend: {}ms", spend.count());
}
