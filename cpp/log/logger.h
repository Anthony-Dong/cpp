#pragma once

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "fmt/core.h"
#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>

namespace cpp::log {
enum class Level {
    Debug,
    Info,
    Warn,
    Error,
};
#define LOG_NAMESPACE cpp::log

// __FILE__ 内置宏
// __LINE__ 内置宏
// __FUNCTION__ 内置宏
// __VA_ARGS__ 内置宏-可变参数
// 这也就是为啥想大部分打印行号信息的库必须要用宏去做的原因了，不过c++20后支持了source_location
#define INFO(...) LOG_NAMESPACE::Log(LOG_NAMESPACE::Level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

template <typename... Args>
void Log(const Level level, const char* filename, int line, const char* func, const fmt::format_string<Args...>& format, Args&&... args) {
    auto file_base = [](const char* name) -> std::string {
        const std::string_view file(name);
        const size_t index = file.find_last_of('/');
        if (index == -1) {
            return name;
        }
        return {file.begin() + index + 1, file.end()};
    };
    using clock = std::chrono::system_clock;
    static std::unordered_map<Level, std::string> level_string_map = {
        {Level::Debug, "DEBUG"},
        {Level::Info, "INFO"},
        {Level::Warn, "WARN"},
        {Level::Error, "ERROR"},
    };
    auto level_name = level_string_map[level];
    if (level_name.empty()) {
        level_name = "-";
    }
    fmt::println("[{}] {} {}:{} {}", level_name, absl::FormatTime("%Y-%m-%d %H:%M:%S%Ez", absl::Now(), absl::LocalTimeZone()), file_base(filename), line, fmt::format(format, std::forward<Args>(args)...));
}

} // namespace cpp::log
