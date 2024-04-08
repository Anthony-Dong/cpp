#!/usr/bin/env bash


# [INFO] 18:42:00.225 Compile: clang++ -std=c++20 -Wall -O0 -g -I/usr/local/include -c compare.cpp -o output/compare.o
  #[INFO] 18:42:01.456 Link: clang++ -o output/compare output/compare.o -L/usr/local/lib -lspdlog -lgtest -lgtest_main -lssl
  #[INFO] 18:42:01.573 Run: output/compare

clang++ -std=c++20 -Wall -O3 -Icmake-build-release-gcc_11/_deps/asio-cmake-src/asio-asio-1-30-0-src/asio/include -Icmake-build-release-gcc_11/_deps/spdlog-src/include -c
