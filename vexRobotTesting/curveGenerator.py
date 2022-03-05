from matplotlib import pyplot as plt
import numpy as np
from random import randint
import math

fig1, axes = plt.subplots(1, 1, figsize=(7, 7))
fig1.suptitle("Map")
fig1.tight_layout()


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def graph(self, color="black"):
        axes.scatter(self.x, self.y, color=color)

    def __str__(self):
        return "(%d, %d)" % (self.x, self.y)


pathPoints = [Point(-15, 0), Point(0, 5), Point(8, 5), Point(15, 0)]
for i in range(0, randint(2, 4)):
    pass  # pathPoints.append(Point(randint(-20, 20), randint(-20, 20)))


def quadraticCurve(Start, End, Control, steps=10):
    stepSize = 1 / steps
    pathGenerated = []
    for t in np.arange(0, 1 + stepSize, stepSize):
        i = 1 - t
        x = (i * i) * Start.x + 2 * i * t * Control.x + (t * t) * End.x
        y = (i * i) * Start.y + 2 * i * t * Control.y + (t * t) * End.y
        pathGenerated.append(Point(x, y))
    return pathGenerated


def getControlPoint(Start, End, P):
    x = 2 * P.x - 0.5 * (Start.x + End.x)
    y = 2 * P.y - 0.5 * (Start.y + End.y)
    return Point(x, y)


def midpoint(P1, P2):
    x = 0.5 * (P1.x + P2.x)
    y = 0.5 * (P1.y + P2.y)
    return Point(x, y)


def generateCurveFromPath(path, steps=10):
    result = [path[0]]
    if len(path) >= 2:
        if len(path) > 2:
            for i in range(1, len(path) - 2):
                r = quadraticCurve(path[i - 1], path[i], getControlPoint(path[i - 1], path[i + 1], path[i]), steps)
                result.extend(r[1:])
            result.extend(quadraticCurve(path[-3], path[-1], getControlPoint(path[-3], path[-1], path[-2]), steps)[1:])
        else:
            return path
    else:
        return []
    return result


class PointPair:
    def __init__(self, x, y, dx, dy):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy

    def graph(self):
        axes.scatter(self.x + self.dx, self.y + self.dy, color="blue")
        axes.scatter(self.x, self.y, color="red")

    def __str__(self):
        return "(%d, %d), <%d, %d>" % (self.x, self.y, self.dx, self.dy)

    def getControlPoint(self):
        return Point(self.x + self.dx, self.y + self.dy)


def generateFromPointPairs(pointPairList, steps=10):
    results = [pointPairList[0]]
    for i in range(0, len(pointPairList) - 1):
        results.extend(quadraticCurve(pointPairList[i], pointPairList[i + 1], pointPairList[i].getControlPoint(),
                                      steps)[1:])
    return results


pairs = [PointPair(0, 0, 5, -10), PointPair(10, -10, 5, 5), PointPair(0, 0, 0, 0)]
t = 0

T = 0.25

while True:
    axes.clear()
    axes.set_ylim(-20, 20)
    axes.set_xlim(-20, 20)
    axes.grid(which="major", color='black', linestyle='-', linewidth=0.5)
    axes.grid(which="minor", color='black', linestyle=':', linewidth=0.5)
    axes.minorticks_on()

    pairs[0].x = 5 * math.cos(5 * t)
    pairs[0].y = 5 * math.sin(5 * t)
    pairs[0].dx = 10 * math.cos(10 * t)
    pairs[0].dy = 2 * math.sin(10 * t)

    pairs[1].x = 5 * math.sin(5 * t)
    pairs[1].y = 5 * math.cos(5 * t)
    pairs[1].dx = 10 * math.cos(10 * t)
    pairs[1].dy = 30 * math.sin(10 * t)

    pairs[2].x = 2 * math.cos(5 * t)
    pairs[2].y = 2 * math.cos(5 * t)
    pairs[2].dx = 2 * math.sin(10 * t)
    pairs[2].dy = 2 * math.sin(10 * t)

    t = t + 2 * 3.14 / T

    for x in generateFromPointPairs(pairs, 20):
        x.graph()
    for p in pairs:
        p.graph()

    plt.pause(T)
