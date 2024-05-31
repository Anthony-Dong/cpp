# vscode 环境

我个人感觉 vscode 开发起来比较舒服，可能是我电脑垃圾带不动 clion！

vscode + clangd(代码提示还是很不错的) 环境非常的简单，只需要下载几个插件

1. clangd https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
2. codelldb: https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb
3. cmake tools: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
4. cmake: https://marketplace.visualstudio.com/items?itemName=twxs.cmake
5. bazel: https://marketplace.visualstudio.com/items?itemName=BazelBuild.vscode-bazel

# clangd 配置

```json
{
  "editor.formatOnSave": true,

  "[cpp]": {
    "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
  },

  // clangd config: https://github.com/clangd/vscode-clangd
  "clangd.path": "/Users/bytedance/software/clangd_18.1.3/bin/clangd", // clangd 本地文件
  "clangd.arguments": [
    "--completion-style=detailed", // 更加详细的信息吧，不希望的可以配置bundled
    "--background-index",
    "--clang-tidy",
    "-j=2",
    "--pch-storage=disk", // 降低内存开销
    "--log=error",
    "--pretty",
    "--compile-commands-dir=output"
  ]
}
```

# cmake 配置

```json
{
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
    // debug配置使用lldb
    "type": "lldb",
    "request": "launch",
    "program": "${command:cmake.launchTargetPath}",
    "args": [],
    "cwd": "${workspaceFolder}"
  }
}
```

# bazel 配置

todo 后续介绍！
