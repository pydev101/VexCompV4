import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial

# initialize serial port
ser = serial.Serial()
ser.port = 'COM7'  # Arduino serial port
ser.baudrate = 9600
ser.timeout = 10  # specify timeout when using readline()

while not ser.is_open:
    try:
        ser.open()
    except Exception as e:
        pass

if ser.is_open:
    print("\nAll right, serial port now open. Configuration:\n")
    print(ser, "\n")  # print serial parameters

# Create figure for plotting
style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)

xs = []
ys = []
frames = 0

# This function is called periodically from FuncAnimation
def animate(i):
    global xs
    global ys
    global frames

    line = ser.readline().decode("UTF-8").replace("\r\n", "")  # ascii
    if line == "":
        return
    if line == "<START>":
        xs = []
        ys = []
        return
    line = line.split(',')
    print(line)
    t = float(line[0])
    p = float(line[1])

    print(t, p)
    xs.append(t)
    ys.append(p)

    if (frames % 50) == 0:
        ax1.clear()
        ax1.plot(xs, ys)
    frames = frames + 1

    if (xs[-1] - xs[0]) > 10:
        del xs[0]
        del ys[0]


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, interval=20)
plt.show()
