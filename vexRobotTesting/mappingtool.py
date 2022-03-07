import pygame
import math
import time
import numpy as np

pygame.init()

WIDTH = 700
HEIGHT = 700
DOT_RADIUS = 5

field_img = pygame.image.load("assets/Field.png")
field_img = pygame.transform.scale(surface=field_img, size=(WIDTH, HEIGHT))

clock = pygame.time.Clock()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
screen.set_alpha(None)

points = []
drawingpoints = []


class Point:
    def __init__(self, x, y, type=3):
        self.x = x
        self.y = y
        self.type = type
        if self.type < 2:
            points.append(self)

    def draw(self, surface):
        if self.type == 0:
            pygame.draw.circle(surface, "red", (self.x, self.y), DOT_RADIUS)
        elif self.type == 1:
            pygame.draw.circle(surface, "blue", (self.x, self.y), DOT_RADIUS)
        else:
            pygame.draw.circle(surface, "black", (self.x, self.y), DOT_RADIUS)

    def collision(self, x, y):
        if math.sqrt((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) <= DOT_RADIUS:
            return True
        return False


def midpoint(xa, ya, xb, yb):
    return (xa + xb) / 2, (ya + yb) / 2

def quadraticCurve(Start, End, Control, steps=10):
    stepSize = 1 / steps
    pathGenerated = []
    for t in np.arange(0, 1 + stepSize, stepSize):
        i = 1 - t
        x = (i * i) * Start.x + 2 * i * t * Control.x + (t * t) * End.x
        y = (i * i) * Start.y + 2 * i * t * Control.y + (t * t) * End.y
        pathGenerated.append(Point(x, y))
    return pathGenerated


running = True
selectedPoint = None
t = 0
while running:
    mx, my = pygame.mouse.get_pos()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            pass
        elif event.type == pygame.MOUSEBUTTONDOWN:
            for p in points:
                if p.collision(mx, my):
                    selectedPoint = p
            if selectedPoint is None:
                t = time.time()
        elif event.type == pygame.MOUSEBUTTONUP:
            if selectedPoint is None:
                if time.time() - t < 0.5:
                    if len(points) > 0:
                        a, b = midpoint(points[-1].x, points[-1].y, mx, my)
                        Point(a, b, 1)
                    Point(mx, my, 0)
            else:
                selectedPoint = None
        elif event.type == pygame.MOUSEMOTION:
            if selectedPoint is not None:
                selectedPoint.x = mx
                selectedPoint.y = my

    screen.blit(field_img, (0, 0))

    if len(points) > 1:
        results = [points[0]]
        for i in range(1, len(points), 2):
            results.extend(quadraticCurve(points[i-1], points[i+1], points[i])[1:])
        for p in results:
            p.draw(screen)
    for p in points:
        p.draw(screen)

    # update the dispalay
    pygame.display.flip()
    clock.tick(30)
