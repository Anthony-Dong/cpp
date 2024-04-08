#include "absl/time/time.h"
#include "absl/time/clock.h"
#include "cpp/utils/base64.h"
#include "spdlog/spdlog.h"

int main() {
    auto noc = absl::GetCurrentTimeNanos();
    auto now = absl::Now();

    spdlog::info("time: {}", absl::FormatTime(now));
    spdlog::info("time size: {}", absl::FormatTime(now).size());
    std::string str = "hello";
    str.push_back('\0');    // good
    str.append("a\0bc", 4); // good
    str = str + "a\0bc";    // bad, 这里会错误的处理成   str = str + "a";

    SPDLOG_INFO("str: {}", cpp::utils::base64_encode(str, false));
}