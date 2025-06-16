.PHONY: all init build test clean

DIR_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

#CC := /usr/bin/clang-15
#CXX := /usr/bin/clang++-15
BUILD_TYPE := Release
BUILD_DIRECTORY := output
VCPKG_HOME := $(HOME)/go/src/github.com/microsoft/vcpkg

all: init build

init:
	mkdir -p "$(BUILD_DIRECTORY)"
	mkdir -p "$(BUILD_DIRECTORY)/install"
	cmake --log-level=DEBUG -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$(BUILD_TYPE)" \
		-DCMAKE_INSTALL_PREFIX="$(DIR_ROOT)$(BUILD_DIRECTORY)/install"  \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake \
		-S . \
		-B "$(BUILD_DIRECTORY)"

build:
	cmake --build "$(BUILD_DIRECTORY)" --config "$(BUILD_TYPE)" -j8

test:
	cd "$(BUILD_DIRECTORY)" && ctest --config "$(BUILD_TYPE)" --tests-regex '_test' -j8

clean:
	rm -rf "$(BUILD_DIRECTORY)"