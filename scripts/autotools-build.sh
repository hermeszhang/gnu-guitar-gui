#!/bin/bash

if [ ! -e "scripts/autotools-build.sh" ]; then
    echo "must run from top level directory"
    exit 1
fi

./configure
if [ $? != 0 ]; then
    exit 1
fi

make
if [ $? != 0 ]; then
    exit 1
fi

