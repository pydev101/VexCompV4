import pygame
import math
import time
import numpy as np

pygame.init()

WIDTH = 900
HEIGHT = 700
DOT_RADIUS = 5

field_img = pygame.image.load("assets/Field.png")
FIELD_WIDTH, FIELD_HEIGHT = 600, 600
field_img = pygame.transform.scale(surface=field_img, size=(FIELD_WIDTH, FIELD_HEIGHT))

clock = pygame.time.Clock()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
screen.set_alpha(None)

points = []


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def draw(self, surface, color):
        pygame.draw.circle(surface, color, (self.x, self.y), DOT_RADIUS)

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


class Movement:
    def __init__(self):
        self.startPoint = None
        self.endPoint = None
        self.endHeading = None
        self.constantVelocity = 0
        self.controlpoints = []
        self.resultpoints = []

        self.guimode = 0
        self.selectedpoint = None
        self.drawpoints = []
        self.t = 0

    def handleEvent(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                if (self.startPoint is None) or (self.endPoint is None):
                    self.t = time.time()
                else:
                    if self.startPoint.collision(mx, my):
                        self.selectedpoint = self.startPoint
                    elif self.endPoint.collision(mx, my):
                        self.selectedpoint = self.endPoint
                    else:
                        for p in self.controlpoints:
                            if p.collision(mx, my):
                                self.selectedpoint = p
                                break
                        if self.selectedpoint is None:
                            self.t = time.time()






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
            if event.button == 1:
                for p in points:
                    if p.collision(mx, my):
                        selectedPoint = p
                        break
                if selectedPoint is None:
                    t = time.time()
            elif event.button == 3:
                if selectedPoint is None:
                    for i in range(0, len(points)):
                        if points[i].collision(mx, my):
                            if points[i].type == 0:
                                if i == 0:
                                    if len(points) > 1:
                                        del points[0]
                                        del points[0]
                                    else:
                                        del points[0]
                                else:
                                    del points[i - 1]
                                    del points[i - 1]
                                break
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

    screen.fill("black")
    screen.blit(field_img, (0, 0))

    if len(points) > 1:
        results = [points[0]]
        for i in range(1, len(points), 2):
            results.extend(quadraticCurve(points[i - 1], points[i + 1], points[i])[1:])
        for p in results:
            p.draw(screen)
    for p in points:
        p.draw(screen)

    # update the dispalay
    pygame.display.flip()
    clock.tick(60)
