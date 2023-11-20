#pragma once

#include <thread>
#include <unistd.h>
#include <chrono>
#include <filesystem>

namespace cpp::utils {

inline uint64_t GetThreadID() {
    static_assert(sizeof(std::thread::id) == sizeof(uint64_t), "this function only works if size of thead::id is equal to the size of uint_64");
    auto id = std::this_thread::get_id();
    auto *ptr = (uint64_t *)&id;
    return (*ptr);
}

inline pid_t GetPID() {
    return getpid();
}

inline std::string GetPWD() {
#if defined(__unix__)
    char buffer[512];
    if (getcwd(buffer, 512) != nullptr) {
        return buffer;
    }
    return "";
#endif
    return std::filesystem::current_path().string();
}
} // namespace cpp::utils
