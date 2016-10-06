#!/bin/bash
for A in a b c d e f g h i j k l m n o p q r s t ; 
    do echo "Test ${A}";
        ./coogsive "input=test-cases/test-${A}.1.input1" | diff - test-cases/test-${A}.out.txt ;
        echo "" ;
        done
