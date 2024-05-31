.PHONY: all init build test clean

DIR_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC := /usr/bin/clang-14
CXX := /usr/bin/clang++-14
BUILD_TYPE := Release
BUILD_DIRECTORY := output

all: init build

init:
	mkdir -p "$(BUILD_DIRECTORY)"
	mkdir -p "$(BUILD_DIRECTORY)/install"
	cmake --log-level=DEBUG -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$(BUILD_TYPE)" \
		-DCMAKE_INSTALL_PREFIX="$(DIR_ROOT)$(BUILD_DIRECTORY)/install"  \
		-DCMAKE_C_COMPILER="$(CC)" \
		-DCMAKE_CXX_COMPILER="$(CXX)" \
		-DCMAKE_EXE_LINKER_FLAGS="-lprofiler -ltcmalloc" \
		-DCMAKE_SHARED_LINKER_FLAGS="-lprofiler -ltcmalloc" \
		-S . \
		-B "$(BUILD_DIRECTORY)"

build:
	cmake --build "$(BUILD_DIRECTORY)" --config "$(BUILD_TYPE)" -j8

test:
	cd "$(BUILD_DIRECTORY)" && ctest --config "$(BUILD_TYPE)" --tests-regex '_test' -j8

clean:
	rm -rf "$(BUILD_DIRECTORY)"