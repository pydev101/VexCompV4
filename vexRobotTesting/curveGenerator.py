from matplotlib import pyplot as plt

fig1, axes = plt.subplots(1, 1, figsize=(7, 7))
fig1.suptitle("Map")
fig1.tight_layout()

axes.set_ylim(-20, 20)
axes.set_xlim(-20, 20)

plt.show(block=False)
x = plt.ginput(4)
print(x)

plt.show()