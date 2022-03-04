from matplotlib import pyplot as plt
import numpy as np
from random import randint

fig1, axes = plt.subplots(1, 1, figsize=(7, 7))
fig1.suptitle("Map")
fig1.tight_layout()

axes.set_ylim(-20, 20)
axes.set_xlim(-20, 20)
axes.grid(which="major", color='black', linestyle='-', linewidth=0.5)
axes.grid(which="minor", color='black', linestyle=':', linewidth=0.5)
axes.minorticks_on()


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
    pass # pathPoints.append(Point(randint(-20, 20), randint(-20, 20)))


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
            for i in range(1, len(path)-2):
                r = quadraticCurve(path[i-1], path[i], getControlPoint(path[i-1], path[i+1], path[i]), steps)
                result.extend(r[1:])
            result.extend(quadraticCurve(path[-3], path[-1], getControlPoint(path[-3], path[-1], path[-2]), steps)[1:])
        else:
            return path
    else:
        return []
    return result

for x in generateCurveFromPath(pathPoints, 20):
    print(x)
    x.graph()
for n in pathPoints:
    n.graph("red")

plt.show()
