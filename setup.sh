#!/bin/bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

$CXX -std=c++11 *cpp -o coogsive || exit 1

./runTests.sh
