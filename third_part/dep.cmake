add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.1.12-stable")
#add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/third_part/fmt-10.1.1")

include (FetchContent)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy (SET CMP0135 NEW)
endif ()

FetchContent_Declare (
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare (
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz
)

#FetchContent_Declare(
#        libevent
#        URL https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz
#)

#FetchContent_Declare (
#        absl
#        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
#        GIT_TAG 20230802.0
#)

FetchContent_Declare (
        absl
        URL https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.tar.gz
)

FetchContent_Declare (
        fmt
        URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz
)

FetchContent_MakeAvailable (googletest spdlog absl fmt)

set (absl_LIBRARIES
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

set (TEST_LIB "GTest::gtest_main")

set (THIRD_PART_LIB fmt spdlog ${absl_LIBRARIES})
message ("THIRD_PART_LIB: ${THIRD_PART_LIB}")