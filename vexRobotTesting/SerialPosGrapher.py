import serial
import struct

import pygame
import math
import time
import numpy as np

pygame.init()

WIDTH = 600
HEIGHT = 600

field_img = pygame.image.load("assets/Vex25.png")
FIELD_WIDTH, FIELD_HEIGHT = 600, 600
SCALING_FACTOR_W, SCALING_FACTOR_H = 142.75/FIELD_WIDTH, 142.75/FIELD_HEIGHT

DOT_RADIUS = 5

field_img = pygame.transform.scale(surface=field_img, size=(FIELD_WIDTH, FIELD_HEIGHT))

clock = pygame.time.Clock()
pygame.display.set_caption("VEX Live")
pygame.display.set_icon(field_img)
screen = pygame.display.set_mode((WIDTH, HEIGHT))
screen.set_alpha(None)

com = serial.Serial("COM4", 9600)

initalPos = (0, 0)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    xyBytes = com.read(8)
    x, y = struct.unpack("<ff", xyBytes)

    x = (x - initalPos[0]) / SCALING_FACTOR_W
    y = (initalPos[1] - y) / SCALING_FACTOR_H

    x = x + 300
    y = y + 300

    screen.blit(field_img, (0, 0))
    pygame.draw.circle(screen, "green", (x, y), DOT_RADIUS)

    pygame.display.flip()
    com.flushInput()
    clock.tick(30)
