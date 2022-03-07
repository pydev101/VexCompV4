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


def midpoint(a, b):
    return (a.x + b.x) / 2, (a.y + b.y) / 2


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
        self.resultpoints = []  # Does not include start/stop points

        self.guimode = 0
        self.selectedpoint = None
        self.t = 0

    def handleEvent(self, event):
        mx, my = pygame.mouse.get_pos()
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
            elif event.button == 3:
                if selectedPoint is None:
                    if self.startPoint is not None:
                        if self.startPoint.collision(mx, my):
                            self.startPoint = None
                            self.controlpoints = []
                            self.calculate()
                    if self.endPoint is not None:
                        if self.endPoint.collision(mx, my):
                            self.endPoint = None
                            self.controlpoints = []
                            self.calculate()
                    for i in range(0, len(self.controlpoints)):
                        if self.controlpoints[i].collision(mx, my):
                            del self.controlpoints[i]
                            self.calculate()
                            break
        elif event.type == pygame.MOUSEBUTTONUP:
            if self.selectedpoint is None:
                if time.time() - self.t < 0.5:
                    if self.startPoint is None:
                        self.startPoint = Point(mx, my)
                    elif self.endPoint is None:
                        self.endPoint = Point(mx, my)
                    else:
                        self.controlpoints.append(Point(mx, my))
                        self.calculate()
            else:
                self.selectedpoint = None
        elif event.type == pygame.MOUSEMOTION:
            if self.selectedpoint is not None:
                self.selectedpoint.x = mx
                self.selectedpoint.y = my
                self.calculate()

    def calculate(self):
        if (self.startPoint is not None) and (self.endPoint is not None) and (len(self.controlpoints) > 0):
            if len(self.controlpoints) == 1:
                self.resultpoints = quadraticCurve(self.startPoint, self.endPoint, self.controlpoints[0])[1:-1]
            else:
                size = len(self.controlpoints)
                midpoints = []
                for i in range(0, size - 1):
                    midpoints.append(midpoint(self.controlpoints[i], self.controlpoints[i+1]))
                # Results extend start to first midpoint, all the midpoints inbetween, then last midpoint to end
                #TODO
        else:
            self.resultpoints = []

    def draw(self):
        if (self.startPoint is not None) and (self.endPoint is not None):
            for p in self.resultpoints:
                p.draw(screen, "black")
            for p in self.controlpoints:
                p.draw(screen, "blue")
        if self.endPoint is not None:
            self.endPoint.draw(screen, "red")
        if self.startPoint is not None:
            self.startPoint.draw(screen, "green")


running = True
selectedPoint = None
t = 0

motion = Movement()
while running:
    mx, my = pygame.mouse.get_pos()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            pass
        motion.handleEvent(event)

    screen.fill("black")
    screen.blit(field_img, (0, 0))

    motion.draw()

    if len(points) > 1:
        results = [points[0]]
        for i in range(1, len(points), 2):
            results.extend(quadraticCurve(points[i - 1], points[i + 1], points[i])[1:])

    # update the dispalay
    pygame.display.flip()
    clock.tick(60)
