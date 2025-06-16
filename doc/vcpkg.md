# vcpkg 

## 介绍

cmake可以无缝集成vcpkg，而vcpkg提供了简单的包管理，提供了几千个CPP的包，基本上主流能使用的包都被vcpkg托管！

你可以像go/rust那样，快速的安装一个依赖，可以使用全局的vcpkg cache，也可以使用`vcpkg.json`来为项目自定义依赖！


## 安装

```shell
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg

./bootstrap-vcpkg.sh
```

## 使用

1. 安装依赖
```shell
vcpkg install fmt
```

2. 搜索依赖
```shell
vcpkg search fmt
```

## cmake 集成

### vscode
在 settings.json 文件中配置
```json
"cmake.configureSettings": {
  "CMAKE_TOOLCHAIN_FILE": "/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
}
```

### clion

```shell
-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```



## vcpkg.json 

```json
{
  "dependencies": [
    "abseil",
    "benchmark",
    "fmt",
    "gtest",
    "libevent",
    "llhttp",
    "protobuf",
    "spdlog"
  ]
}
```

方便项目安装依赖，直接`vcpkg install` 即可！然后就可以为项目单独的隔离依赖版本，会自动下载到当前目录的 `vcpkg_install` 下！