#!bin/bash
#First you can use grep (-n) to find the number of lines of string.
#Then you can use awk to separate the answer.

cat $1 | grep -n $2 | awk -F: '{print $1}' > $3

