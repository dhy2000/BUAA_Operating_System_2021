#!/bin/bash
# ^^^^^^ EXTRA TASK 3 ^^^^^^
xfile=`find test_dir/ | grep xfile`
sed 's/char/int/g' ${xfile} > output
# ^^^^^^ EXTRA TASK 3 DONE ^^^^^^

