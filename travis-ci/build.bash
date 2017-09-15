#!/bin/bash

if [ ! -e "travis-ci/build.bash" ]; then
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

cpack -G TGZ
if [ $? != 0 ]; then
    exit 1
fi

