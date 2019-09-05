###
### compute the jiepai of the function ${1}
###

#echo "$1" >$1.name
#sort -n ../output/gnumc_$1_time.txt >$1.sort
#./com $1.sort $1.sort $1 >$1.time
#paste $1.name $1.time
#rm -rf $1.name $1.time $1 $1.sort

gcc com_basic.c -o com_basic.out
sort -n ${1} > ${1}.sort
./com_basic.out ${1}.sort
rm -rf ${1}.sort
rm com_basic.out

