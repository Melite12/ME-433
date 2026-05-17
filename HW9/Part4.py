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

# Do we need to do till powers of 2??
ts = ts[0:32768]
data = data[0:32768]

Fs = len(ts)/ts[-1]
Ts = 1.0/Fs; # sampling interval
y = data
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig1, (ax1, ax2) = plt.subplots(2, 1)
fig1.suptitle("Signal A")
ax1.plot(ts,y,'b',lw=0.5)
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b',lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')



# SIGNAL B

# Do we need to do till powers of 2??
ts2 = ts2[0:16383]
data2 = data2[0:16383]

Fs = len(ts2)/ts2[-1]
Ts = 1.0/Fs; # sampling interval
y = data2
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig2, (ax1, ax2) = plt.subplots(2, 1)
fig2.suptitle("Signal B")
ax1.plot(ts2,y,'b',lw=0.5)
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b',lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')




# SIGNAL C

# Do we need to do till powers of 2??
#ts3 = ts3[0:16383]
#data3 = data3[0:16383]

Fs = len(ts3)/ts3[-1]
Ts = 1.0/Fs; # sampling interval
y = data3
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig3, (ax1, ax2) = plt.subplots(2, 1)
fig3.suptitle("Signal C")
ax1.plot(ts3,y,'b',lw=0.5)
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b',lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')




# SIGNAL D

# Do we need to do till powers of 2??
# ts4 = ts4[0:4095]
#   data4 = data4[0:4095]

Fs = len(ts4)/ts4[-1]
Ts = 1.0/Fs; # sampling interval
y = data4
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

fig4, (ax1, ax2) = plt.subplots(2, 1)
fig4.suptitle("Signal D")
ax1.plot(ts4,y,'b',lw=0.5)
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b',lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')

plt.show()

