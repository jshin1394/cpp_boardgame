#!/bin/sh

LAST=5
PASS=true
for ((i=1;i<=$LAST;i++)); do
    ./games < in"$i"b.txt > out.txt
	echo out"$i"b.txt
    DIFF=$(diff out.txt out"$i"b.txt)
    if [ "$DIFF" != "" ]; then
       echo "battleship: FAILED test $i!"
       PASS=false
       break
    fi
done
if [ $PASS = true ]; then
    echo "battleship: PASSED all tests"
fi

