import serial
import struct
import time

com = serial.Serial("COM4", 9600)

times = []
xs = []
ys = []

t0 = time.time()
while True:
    xyBytes = com.read(8)
    x, y = struct.unpack("<ff", xyBytes)

    times.append(time.time()-t0)
    xs.append(xs)
    ys.append(ys)


