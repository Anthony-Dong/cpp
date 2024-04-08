.PHONY: all init build test clean

DIR_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC := gcc
CXX := g++
CXX_STANDARD := 17
C_FLAGS :=
CXX_FLAGS := -pthread
BUILD_TYPE := Debug
BUILD_DIRECTORY := output

all: init build

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

build:
	cmake --build "$(BUILD_DIRECTORY)" --config "$(BUILD_TYPE)" -j8

test:
	cd "$(BUILD_DIRECTORY)" && ctest --config "$(BUILD_TYPE)" --tests-regex '_test' -j8

clean:
	rm -rf "$(BUILD_DIRECTORY)"