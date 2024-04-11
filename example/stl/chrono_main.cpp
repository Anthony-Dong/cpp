#include "absl/time/time.h"
#include "absl/time/clock.h"
#include "spdlog/spdlog.h"

int main() {
    auto noc = absl::GetCurrentTimeNanos();
    auto now = absl::Now();

    spdlog::info("time: {}", absl::FormatTime(now));
    spdlog::info("time size: {}", absl::FormatTime(now).size());
}