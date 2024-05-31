cmake_minimum_required(VERSION 3.11)
project(cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # force stdc++=20
set(CMAKE_CXX_EXTENSIONS OFF) # 禁止使用GUN特性

# build --cxxopt -std=c++2b
# build --cxxopt --stdlib=libc++
# build --cxxopt -DASIO_HAS_CO_AWAIT
# build --cxxopt -DASIO_HAS_MOVE
# build --cxxopt -DASIO_HAS_CONSTEXPR
# build --cxxopt -DASIO_HAS_DECLTYPE
# build --cxxopt -DASIO_HAS_STD_SYSTEM_ERROR
# build --cxxopt -Wambiguous-reversed-operator
# build --linkopt -stdlib=libc++
# build --linkopt -lc++abi
# build --linkopt -lrt
# build --linkopt -pthread
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi -pthread")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -lc++abi -pthread")
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

set(CPP_PROJECT_NAME "cpp")
list(APPEND CPP_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/cc_library.cmake)
include(cmake/cc_binary.cmake)
include(cmake/cc_test.cmake)

# 三方依赖
include(third_part/dep.cmake)
include(third_part/asio.cmake)

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
        DEPS cpp::utils spdlog
)