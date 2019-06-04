import subprocess 
import os

sin_gen = "gcc source.c -o source.out"
rc, out = subprocess.getstatusoutput(sin_gen)
performance_test = "gcc gccCorrectnessTest.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out"
performance_run = "./gccCorrectnessTest.out"
main = "./source.out "

start = -3
end = 3
precision = 45
bit = 7
fnum = 1
degree = 6

sin_gen_run = main + ' ' + str(start) + ' ' + str(end) + ' ' + str(precision) + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree)
subprocess.getstatusoutput(sin_gen_run)
#hjw = input("enter any jian if continue")
subprocess.getstatusoutput(performance_test)
rc, out = subprocess.getstatusoutput(performance_run)
print(out)
#temp = [eval(i) for i in out.split()]
#print(temp)