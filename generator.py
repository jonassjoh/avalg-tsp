import random

N = int(input('Number of points: '))
fname = input('File name: ')

with open(fname,'w') as f:
	f.write(str(N))
	for _ in range(N):
		p = str(round(random.uniform(0, 10 ** 6), 4))
		p2 = str(round(random.uniform(0, 10 ** 6), 4))
		f.write('\n' + p + ' ' + p2)
