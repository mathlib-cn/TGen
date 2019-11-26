start=${1}
end=${2}
N=${3}
if [ $# != 3 ]; then
	echo "please input 3 parameters"
	exit
fi
gcc condition_double_number_generator.c binary.c -lm
./a.out ${start} ${end} ${N} 
