#!/bin/bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

$CXX -std=c++11 *cpp -o notify || exit 1
echo ""
echo "test a"
./notify "test-a.1.txt" | diff - test-a.out.txt || exit 1
echo ""
echo "test b"
./notify "test-b.1.txt" | diff - test-b.out.txt || exit 1
echo ""
echo "test z"
./notify "test-z.1.txt" | diff - test-z.out.txt || exit 1
