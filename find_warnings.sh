#!/bin/bash
# 2 nothing change
touch result.txt
gcc -Wall $1 -o test 2>warning.txt
a=$?
grep 'warning' warning.txt >result.txt
sed 's/warning: //g' result.txt >result.txt
if [ $a -eq 0 ]
then
	i=1
	while [ $i -le $2 ]
	do
		echo "$i" | ./test >>result.txt
		i=$[$i+1]
	done
fi
pwd >>result.txt
