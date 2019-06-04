start=${1}
end=${2}
precision=${3}
gcc source.c
gcc gccCorrectnessTest.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o a.out\
&& ./a.out
rm a.out
