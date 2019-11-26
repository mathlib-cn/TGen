import subprocess 
import os
import sys
import math
import numpy as np

# init
sin_gen = "gcc source.c -o source.out"
rc, out = subprocess.getstatusoutput(sin_gen)
main = "./source.out"
data_gen = "./data_gen.sh"
correctness_test = "gcc gccCorrectnessTest_sin.c sin_gen.c binary.c -lm -lgmp -lmpfr -o gccCorrectnessTest_sin.out"
correctness_run = "./gccCorrectnessTest_sin.out"
performance_outputFile = "gcc_sin_time.txt"
performance_test = "gcc gccPerformanceTest_sin.c binary.c -lm -o gccPerformanceTest_sin.out"
performance_run = "./gccPerformanceTest_sin.out" + ' ' + performance_outputFile

temp = []
temp1 = []
temp2 = []
precision_item = []
precisions = []

# set range/constraints and input target information
bit_range = 7
fnum_range = 1
degree_range = 7

# start, end, precision's values don't mind
#start = str(0)
#end = str(1)
#precision = input("please input the precision of computing: ")
start = input("please input the start of interval: ")
end = input("please input the end of interval: ")
N = str(1000000)
precision = str(23)
data_gen = data_gen + ' ' + start + ' ' + end + ' ' + N
print(data_gen)
subprocess.call(data_gen, shell=True)

# generate all possible implementations within the parameters space
# and run the correctness test
# TODO: performance test
for bit in range(0, bit_range + 1):
	for fnum in range(1, fnum_range + 1):
	#for fnum in range(1, 1):
		for degree in range(0, degree_range):
			print("bit = " + str(bit) + " fnum = " + str(fnum) + " degree = " + str(degree))
			sin_gen_run = main + ' ' + start + ' ' + end + ' ' + precision + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree)
			subprocess.getstatusoutput(sin_gen_run)
			newfilename = "sin_gen_" + str(bit) + "_" + str(fnum) + "_" + str(degree) + ".c"
			shellscript = "cp sin_gen.c " + newfilename
			subprocess.getstatusoutput(shellscript)
			correctness_test = "gcc gccCorrectnessTest_sin.c " + newfilename + " binary.c -lm -lgmp -lmpfr -o gccCorrectnessTest_sin.out"
			subprocess.getstatusoutput(correctness_test)
			rc, out = subprocess.getstatusoutput(correctness_run)
			# rc, out = subprocess.getstatusoutput("./testpy.out" + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree))
			print(out)
			temp = [eval(i) for i in out.split()]
			#print(temp)

			precision_item = []
			precision_item.append(temp[0])
			precision_item.append(math.ceil(math.log(temp[0], 2)))
			precision_item.append(temp[3])
			precision_item.append(math.ceil(math.log(temp[3], 2)))
			precision_item.append(bit)
			precision_item.append(degree)
			print(precision_item)
			precisions.append(precision_item)

print("precision is \n" + str(precisions) + "\n")
precisions.sort(key = lambda x : (x[0], x[2]), reverse = True)
print(precisions)
print("\n")
ar = np.array(precisions)
print(ar[:, :])
