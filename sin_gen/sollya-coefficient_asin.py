import sys
import re

if (len(sys.argv) == 2):
	bit = sys.argv[1]
else:
	bit = str(16)
print('coefficient_' + bit + '[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {')
C = ['[|1|]', '[|1,3|]', '[|1,3,5|]', '[|1,3,5,7|]', '[|1,3,5,7,9|]', '[|1,3,5,7,9,11|]', '[|1,3,5,7,9,11,13|]', '[|1,3,5,7,9,11,13,15|]']
f = open("1")
line = f.readline().replace('\n','')
i = 0
while line:
	print()
	print('\t// ' + 'P = fpminimax(asin(x), ' + C[i] + ', [|D...|], [0,sqrt(1 - (' + str(int(bit)-1) + '/' + bit + ')^2)]);')
	print('\t// ' +line)
	print('\t{')
	temp = re.findall(r'(?<=0x)[0-9,a-f]{16}', line)
	temp1 = [('\t\t{.l = 0x' + i + ' },') for i in temp]
	[print(i) for i in temp1]
	line = f.readline().replace('\n','')
	print('\t},')
	i = i + 1
print('};')
