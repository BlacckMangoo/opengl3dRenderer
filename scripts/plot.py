import sys
import matplotlib.pyplot as plt

xs, ys = [], []
with open(sys.argv[1]) as f:
    for line in f:
        x, y = map(float, line.split())
        xs.append(x)
        ys.append(y)

plt.scatter(xs, ys)
plt.show()