## cmake

## 问题

## 1. public 、 private、interface 的区别 [参考](https://chunleili.github.io/cmake/understanding-INTERFACE)

- PRIVATE：只有当前目标使用的头文件，不会导出到外面
- PUBLIC：当前目标和外面都会使用的头文件
- INTERFACE: 接口，很抽象，仅适用于源文件都是头文件的情况，不会编译成动态/静态库，但是自己不会使用

## 2. static / shared 的区别

1. `add_libary` 默认是 static, 现在工程都比较复杂不太推荐使用shared，除非一些系统库

## 3. 一些全局变量介绍

1. `CMAKE_SOURCE_DIR`/`PROJECT_SOURCE_DIR`: 项目根路径
2. `CMAKE_CURRENT_SOURCE_DIR`/`CMAKE_CURRENT_LIST_DIR`: 当前路径, 如果当前目录include(src/CMakeLists.txt)
   时，此时CMAKE_CURRENT_SOURCE_DIR是root，CMAKE_CURRENT_LIST_DIR是src
3. `BUILD_INTERFACE`:
   如果是PUBLIC/INTERFACE表示对外如果有人引用当前目标时需要使用这个include，PRIVATE表示仅在构建自身时使用.  [参考](https://zhuanlan.zhihu.com/p/608943736)
4. `INSTALL_INTERFACE`: 可以是相对路径, 基于 CMAKE_INSTALL_INCLUDEDIR 

## 4. find_package() 问题



1. 搜索路径: /usr/local/lib/cmake
