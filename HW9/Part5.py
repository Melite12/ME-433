import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

ts = []
data = []
with open('dsp/sigA.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts.append(float(row[0]))
        data.append(float(row[1]))

ts2 = []
data2 = []
with open('dsp/sigB.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts2.append(float(row[0]))
        data2.append(float(row[1]))

ts3 = []
data3 = []
with open('dsp/sigC.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts3.append(float(row[0]))
        data3.append(float(row[1]))

ts4 = []
data4 = []
with open('dsp/sigD.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts4.append(float(row[0]))
        data4.append(float(row[1]))


def lpf(BUF_SIZE, TIME, DATA):
    data_lpf = []
    ts_lpf = TIME[BUF_SIZE: len(TIME)]
    for i in range (BUF_SIZE, len(DATA)):
        data_lpf.append(sum(DATA[i - BUF_SIZE: i]) / BUF_SIZE)

    return ts_lpf, data_lpf


# SIGNAL A
ts_lpf, data_lpf = lpf(325, ts, data)

plt.figure()
plt.plot(ts, data, lw=0.5, color ="black")
plt.plot(ts_lpf, data_lpf, lw=0.5, color = "red")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Signal A: ... datapoints")



# SIGNAL B
ts_lpf, data_lpf = lpf(350, ts2, data2)

plt.figure()
plt.plot(ts2, data2, lw=0.5, color ="black")
plt.plot(ts_lpf, data_lpf, lw=0.5, color = "red")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Signal B: ... datapoints")



# SIGNAL C 
# 1 works, but 2500 makes it a triangle (is that smoother)
ts_lpf, data_lpf = lpf(2500, ts3, data3)

plt.figure()
plt.plot(ts3, data3, lw=0.5, color ="black")
plt.plot(ts_lpf, data_lpf, lw=0.5, color = "red")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Signal C: ... datapoints")



# SIGNAL D 
ts_lpf, data_lpf = lpf(100, ts4, data4)

plt.figure()
plt.plot(ts4, data4, lw=0.5, color ="black")
plt.plot(ts_lpf, data_lpf, lw=0.5, color = "red")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Signal D: 100 datapoints")


plt.show()


