import subprocess 
import os
import sys
import math
import struct

# init
target = input("please input the target function type: ")
basicPath = subprocess.getoutput("pwd")
targetPath = "./" + target + "_gen/"
print(targetPath)
generator = "gcc source_" + target + ".c -o source_" + target + ".out"
rc, out = subprocess.getstatusoutput(generator)
main = "./source_" + target + ".out"
data_gen = "./data_gen.sh"
correctness_test = "gcc gccCorrectnessTest_" + target + ".c " + target + "_gen.c binary.c -lm -lgmp -lmpfr -o gccCorrectnessTest_" + target + ".out"
correctness_run = "./gccCorrectnessTest_" + target + ".out"
performance_outputFile = "gcc_" + target + "_time.txt"
performance_test = "gcc gccPerformanceTest_" + target + ".c binary.c -lm -o gccPerformanceTest_" + target + ".out"
performance_run = "./gccPerformanceTest_" + target + ".out" + ' ' + performance_outputFile

temp = []
temp1 = []
temp2 = []
precision_item = []
precisions = []

# set range/constraints and input target information
if target == 'sin':
	bit_range = 7			# [0,1,2,3,4,5,6,7] mean 2^[0,1,2,3,4,5,6,7] for approximate interval
	fnum_range = 1			# 1, dont mind now
	degree_range = 7		# [0,1,2,3,4,5,6] for the highest degree
elif target == 'cos':
	bit_range = 7
	fnum_range = 1
	degree_range = 7
elif target == 'tan':
	bit_range = 7
	fnum_range = 1
	degree_range = 7
elif target == 'exp':
	bit_range = 7
	fnum_range = 1
	degree_range = 8
elif target == "log":
	bit_range = 7
	fnum_range = 1
	degree_range = 8
elif target == "log2":
	bit_range = 7
	fnum_range = 1
	degree_range = 8
elif target == "asin":
	bit_range = 7
	fnum_range = 1
	degree_range = 8
else:
	bit_range = 7
	fnum_range = 1
	degree_range = 7

# set the start & end & N of the test interval; precision dont mind
#start = str(0)
#end = str(1)
#precision = input("please input the precision of computing: ")
start = input("please input the start of interval: ")
end = input("please input the end of interval: ")
N = str(1000000)
precision = str(23)
data_gen = data_gen + ' ' + start + ' ' + end + ' ' + N
subprocess.call(data_gen, shell=True)

# generate all possible implementations within the parameters space
# and run the correctness test
# TODO: performance test
for bit in range(0, bit_range + 1):
	for fnum in range(1, fnum_range + 1):
		for degree in range(0, degree_range):
			# print basic info
			print("bit = " + str(bit) + " fnum = " + str(fnum) + " degree = " + str(degree))
			
			# compile code
			target_gen_run = main + ' ' + start + ' ' + end + ' ' + precision + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree)
			rc, out = subprocess.getstatusoutput(target_gen_run)
			if rc > 0:
				print("compile status: ", rc)
				print(target_gen_run)
				exit(1)
			
			# rename and move file
			newfilename = targetPath + target + "_gen_" + str(bit) + "_" + str(fnum) + "_" + str(degree) + ".c"
			shellscript = "mv " + target + "_gen.c " + newfilename
			rc, out = subprocess.getstatusoutput(shellscript)
			if rc > 0:
				print("rename & move status: ", rc)
				print(out)
				exit(1)

			# correctness test
			correctness_test = "gcc gccCorrectnessTest_" + target + ".c " + newfilename + " binary.c -lm -lgmp -lmpfr -o gccCorrectnessTest_" + target + ".out"
			rc, out = subprocess.getstatusoutput(correctness_test)
			if rc > 0:
				print("correctness_compile: ", rc)
				print(out)
				exit(1)
			rc, out = subprocess.getstatusoutput(correctness_run)
			if rc > 0:
				print("correctness_run: ", rc)
				print(out)
				exit(1)
			# collect function's precision info
			#temp = [eval(i) for i in out.split()]
			temp = out.split()
			maxUlp = struct.unpack('>d', bytes.fromhex(temp[0]))[0]
			averageUlp = struct.unpack('>d', bytes.fromhex(temp[3]))[0]
			if math.isnan(maxUlp) or math.isinf(maxUlp) or math.isnan(averageUlp) or math.isinf(averageUlp):
				print("maxUlp: ", maxUlp)
				print("averageUlp: ", maxUlp)
				continue	
			precision_item = []
			precision_item.append(maxUlp)
			precision_item.append(int(52 - math.ceil(math.log(maxUlp, 2))))
			precision_item.append(averageUlp)
			precision_item.append(int(52 - math.ceil(math.log(averageUlp, 2))))
			precision_item.append(int(bit))
			precision_item.append(int(degree))
			print(precision_item)
			precisions.append(precision_item)

print("precision is")
print(precisions)
precisions.sort(key = lambda x : (x[0], x[2]), reverse = True)
print("after sorting, precisoin is")
print(precisions)
print()

for i in range(len(precisions)):
	temp = precisions[i]
	print("%.8e\t%d\t%.8e\t%d\t%d\t%d"%(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]))
	print()
