#include "spdlog/spdlog.h"

int add(int x, int y) {
    return x + y;
}

int main() {
    SPDLOG_INFO("add(1,2)={}", add(1, 2));
    spdlog::info("hello {}", "cmake");
    SPDLOG_INFO("hello {}", "cmake");
    SPDLOG_INFO("hello {} {}", "world", "!");
}