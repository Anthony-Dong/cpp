# llvm

## mac

```shell
➜  ~  brew install llvm@14
==> Fetching llvm@14
==> Downloading https://ghcr.io/v2/homebrew/core/llvm/14/manifests/14.0.6-3
######################################################################## 100.0%
==> Downloading https://ghcr.io/v2/homebrew/core/llvm/14/blobs/sha256:737b2f91fd
==> Downloading from https://pkg-containers.githubusercontent.com/ghcr1/blobs/sh
######################################################################## 100.0%
==> Pouring llvm@14--14.0.6.monterey.bottle.3.tar.gz
==> Caveats
To use the bundled libc++ please add the following LDFLAGS:
  LDFLAGS="-L/usr/local/opt/llvm@14/lib/c++ -Wl,-rpath,/usr/local/opt/llvm@14/lib/c++"
llvm@14 is keg-only, which means it was not symlinked into /usr/local,
because this is an alternate version of another formula.
If you need to have llvm@14 first in your PATH, run:
  echo 'export PATH="/usr/local/opt/llvm@14/bin:$PATH"' >> ~/.zshrc
For compilers to find llvm@14 you may need to set:
  export LDFLAGS="-L/usr/local/opt/llvm@14/lib"
  export CPPFLAGS="-I/usr/local/opt/llvm@14/include"
==> Summary
🍺  /usr/local/Cellar/llvm@14/14.0.6: 5,827 files, 978.4MB
==> Running `brew cleanup llvm@14`...
Disable this behaviour by setting HOMEBREW_NO_INSTALL_CLEANUP.
Hide these hints with HOMEBREW_NO_ENV_HINTS (see `man brew`).
➜  ~ cd /usr/local/Cellar/llvm@14/14.0.6
```

# linux (debain-10)

1. prepare a debian 10 devbox
2. some packages we need
   1. `sudo apt-get install -y zlib1g-dev ninja-build cmake lsb-release wget software-properties-common gnupg`
3. install clang 14
   1. `wget https://apt.llvm.org/llvm.sh`
   2. `chmod +x llvm.sh`
   3. `sudo ./llvm.sh 14`
4. install libc++ 14
   1. `sudo apt-get install -y libc++abi-14-dev libc++-14-dev`
5. cc/cxx
   CC=/usr/bin/clang-14 CXX=/usr/bin/clang++-14
6. cmake 需要额外配置

```shell
CC := /usr/bin/clang-14
CXX := /usr/bin/clang++-14
CXX_FLAGS := -stdlib=libc++
-DCMAKE_C_COMPILER="/usr/bin/clang-14"
-DCMAKE_CXX_COMPILER="/usr/bin/clang++-14"
-DCMAKE_CXX_FLAGS="-stdlib=libc++"
```

# clangd

clangd 作为 lsp 是可以单独安装的，我平时比较习惯安装新版本的，好处是代码索引做的比较好！直接在这里下载即可 https://github.com/clangd/clangd/releases
