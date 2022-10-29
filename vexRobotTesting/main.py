import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial

print("A")

# initialize serial port
ser = serial.Serial()
ser.port = 'COM12'  # Arduino serial port
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
axes1 = fig.add_subplot(1, 1, 1)

X = []
Y = []
graphs = []


# This function is called periodically from FuncAnimation
def animate(i):
    dataSets = []
    line = ser.readline().decode("UTF-8").replace("\r\n", "")
    while line != "END:END":
        dataSets.append(line)
        line = ser.readline().decode("UTF-8").replace("\r\n", "")

    # print(dataSets)

    graph = [[], [], [], []]
    for input in dataSets:
        label = input.split(":")
        d = label[1].split(",")

        if label[0] == "V":
            # X, Y, dX, dY, Color
            graph[1].append([float(d[0]), float(d[1]), float(d[2]), float(d[3]), d[4]])
        elif label[0] == "P":
            # X, Y, Color
            graph[0].append([float(d[0]), float(d[1]), d[2]])
        elif label[0] == "LPID":
            # d.error < d.pidOutput.output < d.pidOutput.reset < d.realTarget < d.realVel
            graph[2].append([float(d[0]), float(d[1]), float(d[2]), float(d[3]), float(d[4])])
        elif label[0] == "RPID":
            graph[3].append(
                [float(d[0]) * (180 / 3.14159), float(d[1]) * (180 / 3.14159), float(d[2]) * (180 / 3.14159),
                 float(d[3]) * (180 / 3.14159), float(d[4]) * (180 / 3.14159)])
    # graphs.append(graph)

    # print(graphs)

    axes1.clear()
    for v in graph[1]:
        axes1.quiver(v[0], v[1], v[2], v[3], scale=1, angles='xy', scale_units='xy', color=v[4])
    for p in graph[0]:
        axes1.scatter(p[0], p[1], color=p[2])
        if p[2] == "green":
            X.append(p[0])
            Y.append(p[1])
    axes1.plot(X, Y, "gray")
    axes1.set_xlim(-60, 60)
    axes1.set_ylim(-60, 120)
    axes1.grid(which="major", color='black', linestyle='-', linewidth=0.5)
    axes1.grid(which="minor", color='black', linestyle=':', linewidth=0.5)
    axes1.minorticks_on()


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, interval=20)
plt.show()
