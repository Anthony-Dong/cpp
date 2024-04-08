#include <spdlog/spdlog.h>

int main() {
    spdlog::info("hello {}", "world");
    SPDLOG_INFO("hello {}", "world");
}