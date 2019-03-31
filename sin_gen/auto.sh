set -x
gcc data_gen.c binary.c -o data_gen.out
./data_gen.out
rm data_gen.out
sort data.txt > data_temp.txt
mv data_temp.txt data.txt
./test.sh > log_2^-27_pi_${1}_1000000
./test_fpminimax.sh ${1} > fpminimax_log_2^-27_pi_${1}_1000000
