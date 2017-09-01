#!/bin/bash

set -u
set -o errexit

action=$1

for file in include/gnu-guitar-qt/*.h
do
	$action $file
done
