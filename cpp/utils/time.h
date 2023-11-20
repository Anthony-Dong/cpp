#pragma once

#include <chrono>
#include <string>
namespace cpp::utils {

template <class Clock, class Duration>
inline std::string FormatTime(std::chrono::time_point<Clock, Duration> point) {
    static const std::string TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
    using clock = std::chrono::system_clock;
    auto time = clock::to_time_t(point);
    std::string str_format;
    str_format.resize(19);
    if (std::strftime(str_format.data(), str_format.size(), TIME_FORMAT.data(), std::localtime(&time)) == -1) {
        return "";
    }
    return str_format;
}

inline std::string FormatCurrentTime() {
    return FormatTime(std::chrono::system_clock::now());
}

} // namespace cpp::utils