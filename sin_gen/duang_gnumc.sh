#echo "$1" >$1.name
#sort -n ../output/gnumc_$1_time.txt >$1.sort
#./com $1.sort $1.sort $1 >$1.time
#paste $1.name $1.time
#rm -rf $1.name $1.time $1 $1.sort

sort -n ${1}.txt >${1}.sort
./com_basic.out ${1}.sort
rm -rf ${1}.sort

