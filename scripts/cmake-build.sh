#!/bin/bash

if [ ! -e "scripts/cmake-build.sh" ]; then
    echo "must be run from top level directory"
    exit 1
fi

mkdir -p build && cd build
if [ $? != 0 ]; then
  exit 1
fi

cmake ..
if [ $? != 0 ]; then
    exit 1
fi

cmake --build .
if [ $? != 0 ]; then
    exit 1
fi

