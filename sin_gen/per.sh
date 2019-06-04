degree=6
bit=7
fnum=1
for	((bit=1;bit<=7;bit++))
gcc source.c -o source.out \

&& ./source.out ${start} ${end} ${precision} \
&& gcc gccCorrectnessTest.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out \ 
&& ./gccCorrectnessTest.out \
&& rm gccCorrectnessTest.out

