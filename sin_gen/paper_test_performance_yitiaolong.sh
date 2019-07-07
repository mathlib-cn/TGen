gcc gccPerformanceTest.c binary.c -lm -o gccPerformanceTest.out
./gccPerformanceTest.out gcc_sin_time.txt ${1} ${2}
./duang_gnumc.sh gcc_sin_time.txt
