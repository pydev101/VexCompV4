from matplotlib import pyplot as plt

graphs = []


def readData(fileName):
    f = open(fileName, "r")
    raw = f.read()
    f.close()
    lineNumber = 0
    # P, V, LPID, RPID
    graph = [[], [], [], []]
    for line in raw.split("\n"):
        try:
            label = line.split(":")
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
                graph[3].append([float(d[0])*(180/3.14159), float(d[1])*(180/3.14159), float(d[2])*(180/3.14159),
                                 float(d[3])*(180/3.14159), float(d[4])*(180/3.14159)])
            elif label[0] == "END":
                graphs.append(graph)
                graph = [[], [], [], []]
            lineNumber=lineNumber+1
        except Exception as e:
            print(lineNumber)
            raise e


readData("data.csv")

fig2, PIDPlots = plt.subplots(3, 1, figsize=(5, 7))
fig2.suptitle("Linear PID")
PIDPlots[0].title.set_text("Error v Time")
PIDPlots[1].title.set_text("Speed V Time")
PIDPlots[2].title.set_text("Reset V Time")
fig2.tight_layout()

fig3, PIDPlotsRot = plt.subplots(3, 1, figsize=(5, 7))
fig3.suptitle("Rotational PID")
PIDPlotsRot[0].title.set_text("Error v Time")
PIDPlotsRot[1].title.set_text("Speed V Time")
PIDPlotsRot[2].title.set_text("Reset V Time")
fig3.tight_layout()

fig1, axes = plt.subplots(1, 1, figsize=(7, 7))
fig1.suptitle("Map")
fig1.tight_layout()

time = 0
timeTable = []

X = []
Y = []

errorLin = []
linOutput = []
targetLinSpeed = []
realLinSpeed = []
resetLin = []

errorRot = []
rotOutput = []
targetRotSpeed = []
realRotSpeed = []
resetRot = []

plt.show(block=False)
# plt.pause(10)
for i in range(0, len(graphs)):
    # Mention x and y limits to define their range
    graph = graphs[i]
    timeTable.append(time)
    time = time + 0.02*10

    axes.clear()
    for v in graph[1]:
        axes.quiver(v[0], v[1], v[2], v[3], scale=1, angles='xy', scale_units='xy', color=v[4])
    for p in graph[0]:
        axes.scatter(p[0], p[1], color=p[2])
        if p[2] == "green":
            X.append(p[0])
            Y.append(p[1])
    axes.plot(X, Y, "gray")
    axes.set_xlim(-60, 25)
    axes.set_ylim(-10, 100)
    axes.grid(which="major", color='black', linestyle='-', linewidth=0.5)
    axes.grid(which="minor", color='black', linestyle=':', linewidth=0.5)
    axes.minorticks_on()

    for d in graph[2]:
        errorLin.append(d[0])
        linOutput.append(d[1])
        resetLin.append(d[2])
        targetLinSpeed.append(d[3])
        realLinSpeed.append(d[4])
    PIDPlots[0].plot(timeTable, errorLin, color="red")
    PIDPlots[1].plot(timeTable, targetLinSpeed, color="red")
    PIDPlots[1].plot(timeTable, realLinSpeed, color="blue")
    PIDPlots[2].plot(timeTable, resetLin, color="red")
    PIDPlots[0].grid(which="major", color='black', linestyle='-', linewidth=0.5)
    PIDPlots[1].grid(which="major", color='black', linestyle='-', linewidth=0.5)
    PIDPlots[2].grid(which="major", color='black', linestyle='-', linewidth=0.5)

    for d in graph[3]:
        errorRot.append(d[0])
        rotOutput.append(d[1])
        resetRot.append(d[2])
        targetRotSpeed.append(d[3])
        realRotSpeed.append(d[4])
    PIDPlotsRot[0].plot(timeTable, errorRot, color="red")
    PIDPlotsRot[1].plot(timeTable, targetRotSpeed, color="red")
    PIDPlotsRot[1].plot(timeTable, realRotSpeed, color="blue")
    PIDPlotsRot[2].plot(timeTable, resetRot, color="red")
    PIDPlotsRot[0].grid(which="major", color='black', linestyle='-', linewidth=0.5)
    PIDPlotsRot[1].grid(which="major", color='black', linestyle='-', linewidth=0.5)
    PIDPlotsRot[2].grid(which="major", color='black', linestyle='-', linewidth=0.5)
    PIDPlotsRot[0].set_ylim(-90, 90)

    plt.pause(0.5)

plt.show()
