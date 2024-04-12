set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(fmt_example)

include(FetchContent)
set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz
)
FetchContent_Declare(
        fmt
        URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz
)
FetchContent_MakeAvailable(spdlog fmt)


add_executable(main)
target_sources(
        main
        PRIVATE main.cpp
)
target_link_libraries(
        main
        PRIVATE fmt::fmt spdlog
)
