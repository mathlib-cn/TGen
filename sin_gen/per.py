import subprocess 
import os

sin_gen = "gcc source.c -o source.out"
rc, out = subprocess.getstatusoutput(sin_gen)
performance_test = "gcc main_test_sin_gen.c sin_gen.c binary.c computeULP.c -lm -lgmp -lmpfr -o main_test_sin_gen.out"
performance_run = "./main_test_sin_gen.out"
main = "./source.out "
arr = []
max_arr = []
max_times_arr = []
sum_arr = []
max_times_sort = []
min_max_times_indexes = []
min_sum_indexes = []
sum_sort = []

bit_range = 3
fnum_range = 3
degree_range = 6
start = input("please input the start of interval: ")
end = input("please input the end of interval: ")
precision = input("please input the precision of computing: ")

for bit in range(1, bit_range + 1):
	for fnum in range(1, fnum_range + 1):
		for degree in range(0, degree_range):
			print("bit = " + str(bit) + " fnum = " + str(fnum) + " degree = " + str(degree))
			sin_gen_run = main + ' ' + start + ' ' + end + ' ' + precision + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree)
			subprocess.getstatusoutput(sin_gen_run)
			hjw = input("enter any jian if continue")
			subprocess.getstatusoutput(performance_test)
			rc, out = subprocess.getstatusoutput(performance_run)
			#rc, out = subprocess.getstatusoutput("./testpy.out" + ' ' + str(bit) + ' ' + str(fnum) + ' ' + str(degree))
			#print(out)
			temp = [eval(i) for i in out.split()]
			print(temp)
			#arr.append(int(out))
			#temp = list(out)
			#temp.remove('\n')
			#temp.remove('\n')
			#temp.remove('\n')
			#temp = list(map(int, temp))		
			max_arr.append(temp[0])
			max_times_arr.append(temp[1])
			sum_arr.append(temp[2])
			arr.append(temp)
print("arr is " + str(arr))

min_max_indexes = [i for (i,v) in enumerate(max_arr) if v == min(max_arr)]
for i in min_max_indexes:
	max_times_sort.append(max_times_arr[i])
j = 0
for i in min_max_indexes:
	if max_times_sort[j] == min(max_times_sort):
		min_max_times_indexes.append(i)
	j = j + 1
for i in min_max_times_indexes:
	sum_sort.append(sum_arr[i])
j = 0
for i in min_max_times_indexes:
	if sum_sort[j] == min(sum_sort):
		min_sum_indexes.append(i)
	j = j + 1
print(min_sum_indexes)

for i in min_sum_indexes:
	max_index = i
	bit_index = max_index // (fnum_range * degree_range)
	fnum_index = max_index % (fnum_range * degree_range) // fnum_range
	degree_index = max_index % (fnum_range * degree_range) % fnum_range

sin_gen_run = main + ' ' + start + ' ' + end + ' ' + precision + ' ' +str(bit_index) + ' ' + str(fnum_index) + ' ' + str(degree_index)
print(sin_gen_run)
subprocess.getstatusoutput(sin_gen_run)
subprocess.getstatusoutput(performance_test)
rc, out = subprocess.getstatusoutput(performance_run)
temp = [eval(i) for i in out.split()]
print(temp)
#print("max of arr is " + str(max(arr)) + ", and the place is %d" %(max_index))
#print("the number of %d in %s is %d" %(4, str(arr), arr.count(4)))
