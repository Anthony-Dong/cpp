C/C++ 的构建/编译工具有很多，CMake 应该属于第三代构建工具，其次个人觉得应该是 C++生态领域中最广的，一些新一代的虽好但是生态不行！Cmake-Demo 地址: https://github.com/Anthony-Dong/cpp

<!-- more -->

# 介绍

C/C++ 的构建/编译工具有很多，CMake 应该属于第三代构建工具，其次个人觉得应该是 C++生态领域中最广的，一些新一代的虽好但是生态不行！

第一代：g++/gcc/clang，最原始了，构建便大点的项目非常痛苦

第二代：Makefile，通过 Makefile 语法规则简化了命令

第三代：[Autotools](https://www.gnu.org/software/automake/faq/autotools-faq.html) 和 [CMake](https://cmake.org/) 其实就是省略了自己写 Makefile 的过程！

第四代：[Bazel](https://bazel.build/) 、[Blade](https://github.com/chen3feng/blade-build) 集大成者支持任何语言的构建，对于构建缓存支持也好！

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

## 设置C++版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # force stdc++=20
set(CMAKE_CXX_EXTENSIONS OFF) # 禁止使用GUN特性

## 配置clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi -pthread")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -lc++abi -pthread")
endif ()

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
4. 备注: fmt 库可以前往官网自行下载构建 https://fmt.dev/latest/usage.html

# 关键概念

其他注意事项可以看： https://github.com/anthony-dong/cpp/blob/master/doc/cmake_notes.md

## target

Cmake 的所有操作都是围绕着 `target` 走了， `target` 可以是 可执行文件(executable)、静态链接库（static）、动态链接库(shared) !了解了这些你会很方便的理解 Camke 的编写方式！

```cmake
# 定义可执行文件: targe为demo
add_executable(demo make.cpp)

# 定义动态链接库: targe为lib_utils
add_library(lib_utils SHARED utils.cpp)

# 定义静态链接库: targe为lib_random
add_library(${PROJECT_NAME} STATIC random.cpp)
```

备注：INTERFACE 主要是用于那种全部是头文件的构建目标！

## target_xxx_directories

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

# 多模块管理 - 本地链接

一般情况下，如果你是自己本地学习，本地链接是最方便的因为啥了不需要重复编译哇，一次编译处处使用！

这里我大概就自己写了个 demo，大家自行参考，地址：https://github.com/Anthony-Dong/cmake_demo

1. 学会用 cmake 自己安装链接库，这里的例子是 protoc

​ 这里其实很简单，首先这些库一般对于 cmake 都支持，其次就是找到介绍文件，看看它的`CamkeLists.txt` 文件，最后自己看一下哪些需要哪些不需要，最后自己编译即可！具体可以看: https://github.com/Anthony-Dong/cmake_demo

2. 使用 protoc 库

```cmake
project(lib_idl)

add_library(${PROJECT_NAME} SHARED model.pb.cc common.pb.cc utils.cpp)

## link protobuf
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf)
```

3. 定义 pb 文件，使用 protobuf 可以省去了自己写序列化函数的时间，执行命令 `protoc -I . --cpp_out=. common.proto model.proto`

> 注意: protoc 版本必须要和链接库版本一致，不然项目编译报错！

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

# 多模块管理 - FetchContent

FetchContent 比较现代化吧，比较推荐管理外部模块！注意很多时候项目并不是一个 camke 项目，那么你需要自己把这个项目包装成一个 cmake 项目，参考这个实现 https://github.com/kingsamchen/asio-cmake/blob/master/CMakeLists.txt ！

1. `CMakeLists.txt`

```cmake
include (FetchContent)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy (SET CMP0135 NEW)
endif ()

set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER) #强制使用远程模块

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

# 多模块管理 - sub_module

`sub_module` 比较好的是可以将一个项目拆分成多个子模块方便管理，不然一个 `CMakeList.txt` 文件管理整个项目复杂度会高一些！具体可以参考这个项目: [https://github.com/Anthony-Dong/cpp](https://github.com/Anthony-Dong/cpp/)

1. `CMakeLists.txt`

```cmake
cmake_minimum_required (VERSION 3.11)

add_subdirectory (cpp/io) # 引入子模块
```

2. `cpp/io/CMakeLists.txt` 模块内部自己再管理构建目标

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

# vscode + cmake 开发环境

我个人感觉 vscode 开发起来比较舒服，可能是我电脑垃圾带不动 clion！

vscode + clangd(代码提示还是很不错的) 环境非常的简单，只需要下载几个插件

1. clangd https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
2. codelldb: https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb
3. cmake tools: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
4. cmake: https://marketplace.visualstudio.com/items?itemName=twxs.cmake
5. `.vscode/settings.json` 配置

```json
{
  // 开启save后自动format
  "editor.formatOnSave": true,

  // 配置 defaultFormatter 为 clangd
  "[cpp]": {
    "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
  },

  // clangd 配置
  // clangd config: https://github.com/clangd/vscode-clangd
  "clangd.path": "/Users/bytedance/software/clangd_18.1.3/bin/clangd", // 推荐选择最新的clangd下载, 前往这里下载 https://github.com/clangd/clangd/releases
  "clangd.arguments": [
    "--completion-style=detailed", // 更加详细的信息吧，不希望的可以配置bundled
    "--background-index",
    "--fallback-style=Google", // 根据自己需求，或者项目下配置 .clang-format 文件
    "--clang-tidy",
    "-j=2",
    "--pch-storage=disk", // 降低内存开销，能降低2/3左右的内存
    "--log=error",
    "--pretty",
    "--compile-commands-dir=output"
  ],

  // cmake 配置
  // https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md
  "cmake.buildArgs": [],
  "cmake.configureArgs": [
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=1", // 注意(必须开启): enable export clangd compile_commands.json
    "-DCMAKE_BUILD_TYPE=Debug", // debug
    "-DCMAKE_C_COMPILER=/usr/local/opt/llvm@14/bin/clang", // cc
    "-DCMAKE_CXX_COMPILER=/usr/local/opt/llvm@14/bin/clang++" // cxx
  ],
  "cmake.generator": "Unix Makefiles",
  "cmake.buildDirectory": "${workspaceFolder}/output",
  "cmake.debugConfig": {
    // debug使用lldb
    "type": "lldb",
    "request": "launch",
    "program": "${command:cmake.launchTargetPath}",
    "args": [],
    "cwd": "${workspaceFolder}"
  }
}
```

6. 选择构建目标

![img](https://tyut.oss-accelerate.aliyuncs.com/image/2024/5-30/301dc74b063b4f1296eb48215126752c.gif)

7. 运行构建目标

![img](https://tyut.oss-accelerate.aliyuncs.com/image/2024/5-30/6bd43160a9734e12b43f31ac3777a289.gif)

# Bazel

代码示例： https://github.com/Anthony-Dong/bazel_demo

最佳实践：https://anthony-dong.github.io/2023/08/20/1f843b2e9cdc2e9eb8812867e097b659/

Bazel Rule 的话我后期后有问题去写，暂时还没写完！
