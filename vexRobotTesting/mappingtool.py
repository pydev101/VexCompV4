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
SCALING_FACTOR_W, SCALING_FACTOR_H = 142.75/FIELD_WIDTH, 142.75/FIELD_HEIGHT
field_img = pygame.transform.scale(surface=field_img, size=(FIELD_WIDTH, FIELD_HEIGHT))

clock = pygame.time.Clock()
pygame.display.set_caption("VEX Robotics Programming")
pygame.display.set_icon(field_img)
screen = pygame.display.set_mode((WIDTH, HEIGHT))
screen.set_alpha(None)


def drawArrow(screen, Start, End, color=(0, 0, 0)):
    pygame.draw.line(screen, color, (Start.x, Start.y), (End.x, End.y), int(DOT_RADIUS / 2))


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

    def export(self):
        return "Point({}, {})".format(self.x*SCALING_FACTOR_W, self.y*SCALING_FACTOR_H)


def midpoint(a, b):
    return Point((a.x + b.x) / 2, (a.y + b.y) / 2)


def quadraticCurve(Start, End, Control, steps=10):
    stepSize = 1 / steps
    pathGenerated = []
    for t in np.arange(0, 1 + stepSize, stepSize):
        i = 1 - t
        x = (i * i) * Start.x + 2 * i * t * Control.x + (t * t) * End.x
        y = (i * i) * Start.y + 2 * i * t * Control.y + (t * t) * End.y
        pathGenerated.append(Point(x, y))
    return pathGenerated

def calVector(Start, End):
    return "Vector({}, {})".format((End.x - Start.x)*SCALING_FACTOR_W, (End.y - Start.y)*SCALING_FACTOR_H)

class Movement:
    def __init__(self):
        self.startPoint = None
        self.endPoint = None

        self.controlpoints = []
        self.resultpoints = []  # Does not include start/stop points

        self.selectedpoint = None
        self.t = 0

        self.endHeading = None
        self.constantVelocity = 0

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
                if self.selectedpoint is None:
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
        self.resultpoints = []
        if (self.startPoint is not None) and (self.endPoint is not None) and (len(self.controlpoints) > 0):
            if len(self.controlpoints) == 1:
                self.resultpoints = quadraticCurve(self.startPoint, self.endPoint, self.controlpoints[0])[1:-1]
            else:
                size = len(self.controlpoints)
                midpoints = []

                for i in range(0, size - 1):
                    midpoints.append(midpoint(self.controlpoints[i], self.controlpoints[i + 1]))
                # Results extend start to first midpoint, all the midpoints inbetween, then last midpoint to end
                self.resultpoints.extend(quadraticCurve(self.startPoint, midpoints[0], self.controlpoints[0])[1:])
                for i in range(0, size - 2):
                    self.resultpoints.extend(
                        quadraticCurve(midpoints[i], midpoints[i + 1], self.controlpoints[i + 1])[1:])
                self.resultpoints.extend(quadraticCurve(midpoints[-1], self.endPoint, self.controlpoints[-1])[1:-1])

    def draw(self):
        if (self.startPoint is not None) and (self.endPoint is not None):
            lp = self.startPoint
            for p in self.resultpoints:
                drawArrow(screen, lp, p)
                p.draw(screen, "black")
                lp = p
            drawArrow(screen, lp, self.endPoint)
            for p in self.controlpoints:
                p.draw(screen, "blue")
        if self.endPoint is not None:
            self.endPoint.draw(screen, "red")
        if self.startPoint is not None:
            self.startPoint.draw(screen, "green")

    def export(self, realitiveToStart=True):
        results = ""
        if (self.startPoint is not None) and (self.endPoint is not None):
            self.calculate()
            if realitiveToStart:
                pass# Expot start point, everything is in vectors after that; the last vector is from start to end

class MotionProfile:
    def __init__(self):
        self.motion = Movement()


motionProfiles = [MotionProfile()]
currentMotion = motionProfiles[0].motion

running = True
while running:
    mx, my = pygame.mouse.get_pos()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            pass
        currentMotion.handleEvent(event)

    screen.fill("gray")
    screen.blit(field_img, (0, 0))

    currentMotion.draw()

    # update the dispalay
    pygame.display.flip()
    clock.tick(60)
