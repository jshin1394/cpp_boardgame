#!/bin/sh

LAST=5
PASS=true
for ((i= 1; i<=$LAST; i++)); do
    ./games < in"$i"c.txt > out.txt
    DIFF=$(diff out.txt out"$i"c.txt)
    if [ "$DIFF" != "" ]; then
        echo "checker: FAILED test $i!"
        PASS=false
        break
    fi
done
if [ $PASS = true ]; then
    echo "checker: PASSED all tests"
fi