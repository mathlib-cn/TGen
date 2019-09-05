###
### data generate and sort, then run 2 useless correctness test
###

set -x
./data_yitiaolong.sh
#./test.sh > log_2^-27_pi_${1}_1000000.txt
./test.sh > paper_log_${1}_${2}_10000.txt
#./test_fpminimax.sh ${1} > fpminimax_log_2^-27_pi_${1}_1000000.txt
./test_fpminimax.sh ${1} > fpminimax_${1}_${2}_100000.txt
