# 下载和使用
```cmake
include(FetchContent)

FetchContent_Declare(asio-cmake
GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
GIT_TAG        origin/master
)

# Specify asio version
set(ASIO_CMAKE_ASIO_TAG asio-1-30-0)
FetchContent_MakeAvailable(asio-cmake)
```

# 例子

