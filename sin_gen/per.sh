degree=6
bit=7
fnum=1
for	((bit=1;bit<=7;bit++))
gcc source.c -o source.out \

&& ./source.out ${start} ${end} ${precision} \
&& gcc main_test_sin_gen.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o main_test_sin_gen.out \ 
&& ./main_test_sin_gen.out \
&& rm main_test_sin_gen.out

