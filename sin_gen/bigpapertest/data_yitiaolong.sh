###
### generate data and sort it
###

set -x
gcc data_gen.c binary.c -o data_gen.out
./data_gen.out
rm data_gen.out
./data_sort.sh data.txt

