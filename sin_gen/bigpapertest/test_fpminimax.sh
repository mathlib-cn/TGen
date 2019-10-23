###
### correctness test for ${1}.c
###

gcc gccCorrectnessTest.c ${1}.c binary.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out \
&& ./gccCorrectnessTest.out
rm ./gccCorrectnessTest.out
