###################################
###
### compile & run source_exp.c
###
###################################

set -x
gcc source_exp.c -o source_exp.out
./source_exp.out
gcc exp_gen.c -O3 -c
gcc gccCorrectnessTest_exp.c exp_gen.o binary.c computeULP.c -lm -lgmp -lmpfr -o a.out\
&& ./a.out
rm a.out
