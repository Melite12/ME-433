import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

# Part 1
t = []
data = []

with open('dsp/sigA.csv') as f:

    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))
        data.append(float(row[1]))

# Part 2
plt.plot(t,data, lw=0.5)
plt.xlabel("Time (s)")
plt.ylabel("Signal")
plt.title("Signal vs Time")
plt.show()

# Part 3
sample_rate = t[-1]/len(t)
print(sample_rate)

# Part 4

