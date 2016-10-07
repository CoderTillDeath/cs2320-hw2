#!/bin/bash

letter=$1

./coogsive "input=test-cases/test-${letter}.1.input" | diff - test-cases/test-${letter}.out.txt
