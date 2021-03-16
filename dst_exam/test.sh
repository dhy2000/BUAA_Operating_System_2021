# ^^^^^^ TASK 1 ^^^^^^
outfile=lab0_exam.c
# 1-1 grep hello OS lab0
grep "hello OS lab0" dir/dir*/* -n > ${outfile}
# 1-2 find lab0_x
find dir/dir*/lab0_x >> ${outfile}
# ^^^^^^ TASK 1 DONE ^^^^^^
