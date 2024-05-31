#include "spdlog/spdlog.h"

int main() {
    spdlog::info("hello {}", "cmake");
    SPDLOG_INFO("hello {}", "cmake");
    SPDLOG_INFO("hello {} {}", "world", "!");
}