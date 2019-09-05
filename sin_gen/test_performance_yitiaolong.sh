funcname=${1}
start=${2}
end=${3}
gcc gccPerformanceTest_${funcname}.c binary.c -lm -o gccPerformanceTest_${funcname}.out
./gccPerformanceTest_${funcname}.out gcc_${funcname}_time.txt ${start} ${end}
./duang_gnumc.sh gcc_${funcname}_time.txt
