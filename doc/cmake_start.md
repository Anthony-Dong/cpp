# 介绍

C/C++ 的构建/编译工具有很多，CMake应该属于第三代构建工具，其次个人觉得应该是C++生态领域中最广的，一些新一代的虽好但是生态不行！

第一代：g++/gcc/clang，最原始了，构建便大点的项目非常痛苦

第二代：Makefile，通过Makefile语法规则简化了命令

第三代：[Autotools](https://www.gnu.org/software/automake/faq/autotools-faq.html) 和 [CMake](https://cmake.org/) 其实就是省略了自己写Makefile的过程！

第四代：[Bazel](https://bazel.build/) 、[Blade](https://github.com/chen3feng/blade-build)  集大成者支持任何语言的构建，对于构建缓存支持也好！

**注意**：Bazel生态不太行，但是语法比较现代化，不过可以通过 repo_rule 的方式引入cmake项目，但是缺陷就是需要将cmake项目编译成静态库再依赖！

# 快速开始

1. `main.cpp` 文件

```cpp
#include <iostream>
#include <fmt/core.h>

int main() {
    std::cout << "hello world!" << std::endl;
    fmt::print("hello {}!\n", "world");
}
```

2. `CMakeLists.txt` 文件

```cmake
# 设置CMake最低版本
cmake_minimum_required(VERSION 3.8.0)

# 设置项目名称
project(cmake_demo)

## 设置C++版本 和 编译器
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)

## 设置编译选项
## add_compile_options(-Wall -Wextra -pedantic -Werror)

## 设置全局动态链接库目录
link_directories(/usr/local/lib)

## 设置target 为一个可执行文件
add_executable(${PROJECT_NAME} main.cpp)
## 引用的头文件地址
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
## 引用的链接库
target_link_libraries(${PROJECT_NAME} PUBLIC fmt)
```

3. 执行 `cmake .` 即可编译
4. 备注: fmt库可以前往 fmt 官网自行下载构建 https://fmt.dev/latest/usage.html

# 关键概念

## target

Cmake的所有操作都是围绕着 `target` 走了， `target` 可以是 可执行文件(executable)、静态链接库（static）、动态链接库(shared) !了解了这些你会很方便的理解Camke的编写方式！

```cmake
# 定义可执行文件: targe为demo
add_executable(demo make.cpp)

# 定义动态链接库: targe为lib_utils
add_library(lib_utils SHARED utils.cpp)

# 定义静态链接库: targe为lib_random
add_library(${PROJECT_NAME} STATIC random.cpp)
```

## target_xxx_directories

我建议申明为 `PRIVATE`，**遵循使用再引用的原则**

```cmake
# 头文件引用地址
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)

# 动态链接库lookup的地址
target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)
```

## target_link_libraries

这里表示我引用的链接库，这里我建议设置为 `PUBLIC` ，不然别人引用你的库会发现找不到某个函数的定义，那就尴尬了，报错！

**如果有比较好的规范的话我觉得无所谓！**

```cmake
target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)

# 引用的链接库
target_link_libraries(${PROJECT_NAME} PUBLIC fmt)
```

# 多模块管理 -  本地链接

一般情况下，如果你是自己本地学习，本地链接是最方便的因为啥了不需要重复编译哇，一次编译处处使用！

这里我大概就自己写了个demo，大家自行参考，地址：https://github.com/Anthony-Dong/cmake_demo

1. 学会用cmake自己安装链接库，这里的例子是protoc

​	这里其实很简单，首先这些库一般对于cmake都支持，其次就是找到介绍文件，看看它的`CamkeLists.txt` 文件，最后自己看一下哪些需要哪些不需要，最后自己编译即可！具体可以看: https://github.com/Anthony-Dong/cmake_demo

2. 使用 protoc 库

```cmake
project(lib_idl)

add_library(${PROJECT_NAME} SHARED model.pb.cc common.pb.cc utils.cpp)

## link protobuf
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf)
```

3. 定义pb文件，使用protobuf可以省去了自己写序列化函数的时间，执行命令  `protoc -I . --cpp_out=. common.proto model.proto`

> 注意: protoc版本必须要和链接库版本一致，不然项目编译报错！

```protobuf
//  model.proto
syntax = "proto2";

package model.idl;

import "common.proto";

enum Gendor {
  Female = 1;
  Male = 2;
}

message People {
  optional int64 ID = 1;
  optional string Name = 2;
  optional Gendor Gendor = 3;
  repeated string ExtralList = 4;
  map<string, string> Extra = 5;
  optional ExtraInfo ExtraInfo = 6;
  optional Status status = 7;
}

message ExtraInfo {optional string name = 1;}


// common.proto
syntax = "proto2";

package model.idl;

enum Status {
  On = 0;
  Off = 1;
}
```

4. 主函数

```cpp
#include <fmt/core.h>
#include <idl/model.pb.h>
#include <idl/utils.h>

#include <db/Class.h++>
#include <iostream>

int main() {
    std::cout << "hello world!" << std::endl;
    fmt::print("hello {}!\n", "world");

    DB::Model::Class a(1, "1314班");
    fmt::print("id: {}, name: {}\n", a.getId(), a.getName());

    model::idl::People people{};
    people.set_name("tom");
    people.set_id(1);
    fmt::print("people: {}\n", model::idl::toJson(people));
    fmt::print("people: {}\n", *model::idl::toJsonPtr(people));
}
```

# 多模块管理 -  FetchContent

FetchContent 比较现代化吧，比较推荐管理外部模块！

1. `CMakeLists.txt`

```cmake
include (FetchContent)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy (SET CMP0135 NEW)
endif ()

FetchContent_Declare (
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz
)

FetchContent_MakeAvailable (spdlog)

add_library(main main.cpp)
target_link_libraries(main PUBLIC spdlog)
```

2. main.cpp

```cpp
#include "spdlog/spdlog.h"

int main() {
    spdlog::info("hello {}", "cmake");
    SPDLOG_INFO("hello {}", "cmake");
}
```

# 多模块管理 -  sub_module

`sub_module` 比较现代化，比较推荐管理子模块，很多cmake项目是全局一个cmakelists.txt 文件维护构建信息，导致庞大。sub_module 可以添加子模块的方式去解决！具体可以参考这个项目: [https://github.com/Anthony-Dong/cpp](https://github.com/Anthony-Dong/cpp/)

`add_subdirectory`  仅需要模块目录定义 `CMakeLists.txt` 文件即可！

1. `CMakeLists.txt`

```cmake
cmake_minimum_required (VERSION 3.11)

include (cmake/cc_library.cmake)
include (cmake/cc_binary.cmake)
include (cmake/cc_test.cmake)

add_subdirectory (cpp/io)
add_subdirectory (cpp/utils)
add_subdirectory (cpp/network)
add_subdirectory (cpp/log)
add_subdirectory (example)
add_subdirectory (test)
```

2. `cpp/io/CMakeLists.txt`

```cmake
cc_library (
        NAME cpp_io
        ALIAS cpp::io
        SRCS io.cpp
        HDRS io.h
        DEPS cpp::utils
)

cc_test (
        NAME cpp_io_test
        SRCS io_test.cpp
        DEPS cpp::io
)
```

# 远程调试

远程调试的能力依赖于gdbserver，其次 cmake构建的时候需要指定 `-DCMAKE_BUILD_TYPE=Debug`

> Debug：用于在没有优化的情况下，使用带有调试符号构建库或者可执行文件

```shell
~/go/src/github.com/anthony-dong/cmake_demo gdbserver :10086 output/cmake_demo
Process output/cmake_demo created; pid = 149849
Listening on port 10086
Remote debugging from host 10.78.117.128
now 2023-08-18 17:28:22
hello world!
```

![image-20230818173037509](https://tyut.oss-accelerate.aliyuncs.com/image/2023/8-18/c05a4e0255fd4e4891cfc062c73266c1.png)2. 效果

![image-20230818173057174](https://tyut.oss-accelerate.aliyuncs.com/image/2023/8-18/d7a1664672114e47acc4b8040be97638.png)

# cmake 构建命令

Makefile 中定义了 `build` 和   `test`指令，也是cmake中场景的命令

```makefile
.PHONY: all init build test clean

DIR_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC := clang
CXX := clang++
CXX_STANDARD := 17
C_FLAGS :=
CXX_FLAGS := -pthread
BUILD_TYPE := Debug
BUILD_DIRECTORY := output

all: build

init:
	mkdir -p "$(BUILD_DIRECTORY)"
	mkdir -p "$(BUILD_DIRECTORY)/install"
	cmake --log-level=DEBUG -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$(BUILD_TYPE)" \
		-DCMAKE_INSTALL_PREFIX="$(DIR_ROOT)$(BUILD_DIRECTORY)/install"  \
		-DCMAKE_C_COMPILER="$(CC)" \
		-DCMAKE_CXX_COMPILER="$(CXX)" \
		-DCMAKE_C_FLAGS="$(C_FLAGS)" \
		-DCMAKE_CXX_FLAGS="$(CXX_FLAGS)" \
		-DCMAKE_CXX_STANDARD="$(CXX_STANDARD)" \
		-DABSL_PROPAGATE_CXX_STD=ON \
		-S . \
		-B "$(BUILD_DIRECTORY)"

build: init
	cmake --build "$(BUILD_DIRECTORY)" --config "$(BUILD_TYPE)" -j8

test:
	cd "$(BUILD_DIRECTORY)" && ctest --config "$(BUILD_TYPE)" --tests-regex '_test' -j8

clean:
	rm -rf "$(BUILD_DIRECTORY)"
```

# Bazel

代码示例： https://github.com/Anthony-Dong/bazel_demo

最佳实践：https://anthony-dong.github.io/2023/08/20/1f843b2e9cdc2e9eb8812867e097b659/

Bazel Rule 的话我后期后有问题去写，暂时还没写完！