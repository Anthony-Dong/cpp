cmake_minimum_required(VERSION 3.11)
project(cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # force stdc++=20
set(CMAKE_CXX_EXTENSIONS OFF) # 禁止使用GUN特性
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# link: -fuse-ld=lld
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -pthread")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -pthread")
endif ()

enable_testing()

message("CMAKE_VERSION: ${CMAKE_VERSION}")
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND CUSTOM_PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include(cmake/cc_library.cmake)
include(cmake/cc_binary.cmake)
include(cmake/cc_test.cmake)

# 三方依赖
find_package(absl CONFIG REQUIRED)
find_package(Libevent CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(llhttp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)

set(lib_gsl Microsoft.GSL::GSL)
set(lib_protobuf protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
set(lib_benchmark benchmark::benchmark benchmark::benchmark_main)
set(lib_llhttp llhttp::llhttp_static)
set(lib_libevent libevent::core libevent::extra libevent::pthreads)
set(lib_asio asio::asio)
set(lib_fmt fmt::fmt)
set(lib_spdlog spdlog::spdlog)
set(lib_gtest GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
set(lib_absl
#    absl::algorithm

    # absl::base
    # absl::debugging
    # absl::btree
    # absl::hash
    # absl::memory
    # absl::meta
    # absl::numeric
    # absl::status
    absl::strings

    # absl::synchronization
    absl::time
#    absl::utility
)

# 自己的模块
add_subdirectory(cpp/io)
add_subdirectory(cpp/utils)
add_subdirectory(cpp/network)
add_subdirectory(cpp/log)
add_subdirectory(cpp/http)
add_subdirectory(example)
add_subdirectory(test)

cc_binary(
    NAME main
    SRCS main.cpp
    DEPS cpp::utils ${lib_spdlog} ${lib_absl} ${lib_fmt} ${lib_protobuf} ${lib_gsl}
)