gcc main_test_sin_gen.c model_fpminimax_pi_${1}.c binary.c computeULP.c -lm -lgmp -lmpfr -o main_test_sin_gen.out \
&& ./main_test_sin_gen.out
rm ./main_test_sin_gen.out
