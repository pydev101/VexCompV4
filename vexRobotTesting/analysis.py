from matplotlib import pyplot as plt

graphs = []


def readData(fileName):
    f = open(fileName, "r")
    raw = f.read()
    f.close()

    # P, V
    graph = [[], []]
    for line in raw.split("\n"):
        d = line.split(":")
        if d[0] == "V":
            d = d[1].split(",")
            graph[1].append([float(d[0]), float(d[1]), float(d[2]), float(d[3]), d[4]])
        elif d[0] == "P":
            d = d[1].split(",")
            graph[0].append([float(d[0]), float(d[1]), d[2]])
        elif d[0] == "END":
            graphs.append(graph)
            graph = [[], []]


readData("data.csv")

fig1, axes = plt.subplots(1, 1)
fig2, PIDPlots = plt.subplots(2, 1)
fig1.suptitle("Map")
fig2.suptitle("PID")
PIDPlots[0].title.set_text("Linear")
PIDPlots[1].title.set_text("Rotational")

X = []
Y = []
for i in range(0, len(graphs)):
    # Mention x and y limits to define their range
    graph = graphs[i]

    axes.clear()

    for v in graph[1]:
        axes.quiver(v[0], v[1], v[2], v[3], scale=1, angles='xy', scale_units='xy', color=v[4])
    for p in graph[0]:
        axes.scatter(p[0], p[1], color=p[2])
        if p[2] == "green":
            X.append(p[0])
            Y.append(p[1])
    axes.plot(X, Y, "gray")

    axes.set_xlim(-25, 25)
    axes.set_ylim(-10, 30)
    axes.grid(which="major", color='black', linestyle='-', linewidth=0.5)
    axes.grid(which="minor", color='black', linestyle=':', linewidth=0.5)
    axes.minorticks_on()
    plt.pause(0.5)

plt.show()
