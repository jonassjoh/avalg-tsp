import math
import sys

class Point:
    def __init__(self, x, y):
        self.x = x;
        self.y = y;

    def __str__(self):
        return '(' + str(self.x) + ', ' + str(self.y) + ')'

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

N = int(input())

points = []
for i in range(N):
    p = input().split(' ')
    points.append(Point( float(p[0]), float(p[1]) ))

tour = greedyTour(points)
for t in tour:
    print(t)
