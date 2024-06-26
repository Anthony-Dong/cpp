#find_package(libevent)
#if(libevent_FOUND)
#    message(STATUS "libevent found")
#else()
#    message(FATAL_ERROR "libevent not found")
#endif()

include(FetchContent)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

# mac下使用 find_package 有点问题
if (APPLE)
    set(EVENT__FORCE_KQUEUE_CHECK ON)
endif ()

set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz
)

#add_definitions(-DFMT_HEADER_ONLY)
FetchContent_Declare(
        fmt
        URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz
)

set(EVENT__DISABLE_SAMPLES TRUE CACHE BOOL "" FORCE)
set(EVENT__DISABLE_TESTS TRUE CACHE BOOL "" FORCE)
set(EVENT__DISABLE_BENCHMARK TRUE CACHE BOOL "" FORCE)
FetchContent_Declare(
        libevent
        GIT_REPOSITORY https://github.com/libevent/libevent.git
        GIT_TAG release-2.1.12-stable
)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_USE_BUNDLED_GTEST OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
)

set(ABSL_PROPAGATE_CXX_STD ON)
FetchContent_Declare(
        absl
        URL https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.tar.gz
)


FetchContent_MakeAvailable(fmt spdlog googletest googlebenchmark absl libevent)

set(absl_LIBRARIES
        absl::algorithm
        absl::base
        absl::debugging
        absl::btree
        absl::hash
        absl::memory
        absl::meta
        absl::numeric
        absl::status
        absl::str_format
        absl::strings
        absl::synchronization
        absl::time
        absl::utility)

set(TEST_LIB "GTest::gtest_main")

set(THIRD_PART_LIB fmt spdlog ${absl_LIBRARIES})
message("THIRD_PART_LIB: ${THIRD_PART_LIB}")

FetchContent_Declare(
        llhttp
        URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.2.1.tar.gz"
)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(llhttp)
# llhttp_static