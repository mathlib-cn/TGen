###
### correctness test for model_7.c
###

gcc gccCorrectnessTest.c model_7.c binary.c computeULP.c -lm -lgmp -lmpfr \
&& ./a.out
rm a.out
