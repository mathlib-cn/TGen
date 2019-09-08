###################################
###
### compile & run source.c
###
###################################

set -x
gcc source.c -o source.out
./source.out
gcc gccCorrectnessTest_exp.c exp_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o a.out\
&& ./a.out
rm a.out
