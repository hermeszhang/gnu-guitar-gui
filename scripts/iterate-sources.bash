#!/bin/bash

set -u
set -o errexit

action=$1

for file in src/*.cc
do
  $action $file
done
