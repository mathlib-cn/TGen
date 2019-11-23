###
### generate data and sort it
###
set -x

#init
start=${1}
end=${2}

gcc data_gen.c binary.c -lm -o data_gen.out
./data_gen.out ${start} ${end}
rm data_gen.out
./data_sort.sh data.txt

