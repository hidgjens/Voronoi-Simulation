import matplotlib as mpl
from matplotlib import pyplot as plt
import numpy as np
import time

x, y = np.random.rand(2,3)

plt.ion()

fig = plt.figure()
ax = fig.add_subplot(111)
line1, = ax.plot(x, y, linestyle='', marker='.') # Returns a tuple of line objects, thus the comma
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)

for i in range(10):
    x, y = np.random.rand(2,3)
    
    line1.set_xdata(x)
    line1.set_ydata(y)

    fig.canvas.draw()
    fig.canvas.flush_events()
    time.sleep(1)