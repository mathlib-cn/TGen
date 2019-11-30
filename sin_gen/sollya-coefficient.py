import sys
import re

if (len(sys.argv) == 2):
	bit = sys.argv[1]
else:
	bit = str(128)
print('coefficient_1div_' + bit + '[COEFFICIENTS][COEFFICIENTS] = {')
C = ['[|1|]', '[|1,2|]', '[|1,2,3|]', '[|1,2,3,4|]', '[|1,2,3,4,5|]', '[|1,2,3,4,5,6|]', '[|1,2,3,4,5,6,7|]', ]
f = open("1")
line = f.readline().replace('\n','')
print('\t{')
print('\t\t{.l = 0},')
print('\t},')
i = 0
while line:
	print()
	print('\t// ' + 'P = fpminimax(log(1+x), ' + C[i] + ', [|D...|], [1b-53,1/' + bit + ']);')
	print('\t// ' +line)
	print('\t{')
	print('\t\t{.l = 0},')
	temp = re.findall(r'(?<=0x)[0-9,a-f]{16}', line)
	temp1 = [('\t\t{.l = 0x' + i + ' },') for i in temp]
	[print(i) for i in temp1]
	line = f.readline().replace('\n','')
	print('\t},')
	i = i + 1
print('};')
