import subprocess 
import os
import sys

# init
sin_gen = "gcc source.c -o source.out"
rc, out = subprocess.getstatusoutput(sin_gen)
main = "./source.out"
data_gen = "./data_gen.sh"
correctness_test = "gcc gccCorrectnessTest.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out"
correctness_run = "./gccCorrectnessTest.out"
performance_outputFile = "gcc_sin_time.txt"
performance_test = "gcc gccPerformanceTest.c binary.c -lm -o gccPerformanceTest.out"
performance_run = "./gccPerformanceTest.out" + ' ' + performance_outputFile

arr = []
max_arr = []
max_times_arr = []
sum_arr = []
max_times_sort = []
min_max_times_indexes = []
min_sum_indexes = []
sum_sort = []
correctness_value = []
correctness_list = []

# set range/constraints and input target information
bit_range = 7
fnum_range = 1
degree_range = 7

start = input("please input the start of interval: ")
end = input("please input the end of interval: ")
N = str(1000000)
precision = input("please input the precision of computing: ")
data_gen = data_gen + ' ' + start + ' ' + end + ' ' + N
print(data_gen)
subprocess.call(data_gen, shell=True)

# ulp_max_tolerate
ulp_max_tolerate = 1 << (53 - int(precision))

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
			correctness_test = "gcc gccCorrectnessTest.c " + newfilename + " binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out"
			subprocess.getstatusoutput(correctness_test)
			rc, out = subprocess.getstatusoutput(correctness_run)
			# rc, out = subprocess.getstatusoutput("./testpy.out" + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree))
			print(out)
			temp = [eval(i) for i in out.split()]
			print(temp)		
			max_arr.append(temp[0])
			max_times_arr.append(temp[1])
			sum_arr.append(temp[2])
			arr.append(temp)
			
			# find these version which is not bigger than ulp_max_tolerate
			if temp[0] < ulp_max_tolerate:
				correctness_value = []
				correctness_value.append(bit)
				correctness_value.append(fnum)
				correctness_value.append(degree)
				correctness_value.append(temp[0])
				correctness_value.append(temp[1])
				correctness_value.append(temp[2])
				correctness_list.append(correctness_value)
			print()
# print("arr is " + str(arr))


# select the best one
bit_list = []
min_bit_correctness_list = []
fnum_list = []
min_fnum_correctness_list = []
degree_list = []
for correctness_value in correctness_list:
	bit = correctness_value[0]
	bit_list.append(bit)
min_bit = min(bit_list)
for correctness_value in correctness_list:
	if correctness_value[0] == min_bit:
		min_bit_correctness_list.append(correctness_value)
		fnum = correctness_value[1]
		fnum_list.append(fnum)
min_fnum = min(fnum_list)
for correctness_value in min_bit_correctness_list:
	if correctness_value[1] == min_fnum:
		min_fnum_correctness_list.append(correctness_value)
		degree = correctness_value[2]
		degree_list.append(degree)
min_degree = min(degree_list)

print("precision = ", precision, " ulp_max_tolerate = ", ulp_max_tolerate)
print("correctness_list:")
print(correctness_list)
print("min_bit: " + str(min_bit) + " min_fnum: " + str(min_fnum) + " min_degree: " + str(min_degree))
print('')
# select end

min_bit_indexes = [i for (i,v) in enumerate(bit_list) if v == min(bit_list)]

# search for the best parameter vector by correctness
min_max_indexes = [i for (i,v) in enumerate(max_arr) if v == min(max_arr)]
#print("min_max_indexes")
#print(min_max_indexes)
for i in min_max_indexes:
	max_times_sort.append(max_times_arr[i])
#print("max_times_sort")
#print(max_times_sort)
j = 0
for i in min_max_indexes:
	if max_times_sort[j] == min(max_times_sort):
		min_max_times_indexes.append(i)
	j = j + 1
for i in min_max_times_indexes:
	sum_sort.append(sum_arr[i])
#print("sum_sort")
#print(sum_sort)
j = 0
#print("min_max_times_indexes")
#print(min_max_times_indexes)
for i in min_max_times_indexes:
	if sum_sort[j] == min(sum_sort):
		min_sum_indexes.append(i)
	j = j + 1
#print("min_sum_indexes")
#print(min_sum_indexes)

# compute the index of the best parameter vector
for i in min_sum_indexes:
	max_index = i
	bit_index = max_index // (fnum_range * degree_range) + 0
	fnum_index = max_index % (fnum_range * degree_range) // degree_range + 1
	degree_index = max_index % (fnum_range * degree_range) % degree_range + 0

	print("bit_index: " + str(bit_index) + " fnum_index: " + str(fnum_index) + " degree_index: " + str(degree_index))

	# generate the final selected code and print some neccessary information
	sin_gen_run = main + ' ' + start + ' ' + end + ' ' + precision + ' ' +str(bit_index) + ' ' + str(fnum_index) + ' ' + str(degree_index)
	subprocess.getstatusoutput(sin_gen_run)
	correctness_test = "gcc gccCorrectnessTest.c " + "sin_gen.c" + " binary.c computeULP.c -lm -lgmp -lmpfr -o gccCorrectnessTest.out"
	subprocess.getstatusoutput(correctness_test)
	rc, out = subprocess.getstatusoutput(correctness_run)
	temp = [eval(i) for i in out.split()]
	print(temp)
	print()

#print("max of arr is " + str(max(arr)) + ", and the place is %d" %(max_index))
#print("the number of %d in %s is %d" %(4, str(arr), arr.count(4)))
