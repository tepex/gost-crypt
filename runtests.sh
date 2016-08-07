#!/bin/sh

echo "Running unit tests:"

for i in build/*_tests
do
	if test -f $i
	then
		if $VALGRIND ./$i 2>> logs/tests.log
		then
			echo $i PASS
		else
			echo "ERROR in test $i: here's tests.log"
			echo "------"
			cat logs/tests.log
			exit 1
		fi
	fi
done

echo ""
