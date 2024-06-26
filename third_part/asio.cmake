if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy (SET CMP0135 NEW)
endif ()

include(FetchContent)

FetchContent_Declare(asio-cmake
        GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
        GIT_TAG        origin/master
)

# Specify asio version
set(ASIO_CMAKE_ASIO_TAG asio-1-30-0)
FetchContent_MakeAvailable(asio-cmake)

