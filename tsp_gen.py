import random
import math
import sys

class Point:
    def __init__(self, x, y):
        self.x = x;
        self.y = y;

    def __str__(self):
        return str(self.x) + ' ' + str(self.y) + '\n'

    def printSelf(self):
        print(str(self))

    def distance(self, p):
        return int(math.sqrt( math.pow(self.x - p.x, 2) + math.pow(self.y - p.y, 2) ) + 0.5)

def dist(points, p1, p2):
    return points[p1].distance(points[p2])

def greedyTour(points):
    tour = [0] * len(points)
    used = [False] * len(points)
    used[0] = True

    for i in range(1, len(points)):
        best = -1
        for j in range(len(points)):
            if not used[j] and (best == -1 or dist(points, tour[i-1], j) < dist(points, tour[i-1], best)):
                best = j
        tour[i] = best
        used[best] = True
    return tour

N = 100
a = 10 ** 6
b = 10 ** 5

f_r = str(random.randrange(10000))

f_name_in = 'sample_' + f_r + '.in'
f_name_out = 'sample_' + f_r + '.out'

points = []
for i in range(N):
    points.append(Point( random.uniform(a, b), random.uniform(a, b) ))

with open(f_name_in, 'w') as f:
    f.write(str(N) + '\n')
    for p in points:
        f.write( str(p) )

tour = greedyTour(points)
with open(f_name_out, 'w') as f:
    for t in tour:
        f.write( str(t) + '\n' )
