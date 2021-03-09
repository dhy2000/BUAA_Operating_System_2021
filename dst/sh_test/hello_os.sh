#!/bin/bash
AAA=$1
BBB=$2
# echo "">$BBB

flg=0

lines=(8 32 128 512 1024)

for line in ${lines[*]}
do
# ln=`sed -n ''${line}'p' $AAA`

if [ $flg -eq 0 ]; then
sed -n ''${line}'p' $AAA > $BBB
else
sed -n ''${line}'p' $AAA >> $BBB
fi
flg=1

done

# echo $ans > $BBB
# echo $ans
