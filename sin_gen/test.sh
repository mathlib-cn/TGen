gcc main_test_sin_gen.c model_7.c binary.c computeULP.c -lm -lgmp -lmpfr \
&& ./a.out
rm a.out
