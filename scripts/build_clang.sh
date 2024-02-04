#!/bin/bash

cd $(dirname $0)
set -e

pushd ..

BUILD_DIR=./build
rm -r -f $BUILD_DIR
mkdir -p $BUILD_DIR

EXTRA_FLAGS="-Werror -Wall -g -O0"

clang++ main.cpp $EXTRA_FLAGS -std=c++17 -Ilibs/raylib/Debug/include -L./libs/raylib/Debug/lib -lraylib -Wl,-framework,cocoa -Wl,-framework,iokit -o $BUILD_DIR/smooth_life

popd