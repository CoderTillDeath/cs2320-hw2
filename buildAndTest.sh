#!/bin/bash

letter=$1

./compile.sh
./output.sh ${letter} | diff - test-cases/test-${letter}.out.txt

