#include <iostream>
#include <fmt/core.h>
#include <cmath>
#include <string>
#include <chrono>
#include "cpp/utils/time.h"
#include <unordered_map>

namespace cpp::log {
enum class Level {
    Debug,
    Info,
    Warn,
    Error,
};
#define LOG_NAMESPACE cpp::log

#define INFO(...) LOG_NAMESPACE::Log(LOG_NAMESPACE::Level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

template <typename... Args>
inline void Log(Level level, const char *filename, int line, const char *func, fmt::format_string<Args...> format, Args &&...args) {
    auto file_base = [](const char *name) -> std::string {
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
    auto &level_name = level_string_map[level];
    if (level_name.empty()) {
        level_name = "-";
    }
    fmt::println("[{}] {} {}:{} {}", level_name, cpp::utils::FormatTime(clock::now()), file_base(filename), line, fmt::format(format, std::forward<Args>(args)...));
}

} // namespace cpp::log
