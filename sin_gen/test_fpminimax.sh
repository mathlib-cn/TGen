gcc gccCorrectnessTest.c model_fpminimax_pi_${1}.c binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out \
&& ./gccCorrectnessTest.out
rm ./gccCorrectnessTest.out
