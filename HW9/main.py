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
ts = ts[0:32768]
data = data[0:32768]

ts2 = []
data2 = []
with open('dsp/sigB.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts2.append(float(row[0]))
        data2.append(float(row[1]))
ts2 = ts2[0:16383]
data2 = data2[0:16383]

ts3 = []
data3 = []
with open('dsp/sigC.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts3.append(float(row[0]))
        data3.append(float(row[1]))
ts3 = ts3[0:16383]
data3 = data3[0:16383]

ts4 = []
data4 = []
with open('dsp/sigD.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        ts4.append(float(row[0]))
        data4.append(float(row[1]))
ts4 = ts4[0:4095]
data4 = data4[0:4095]



def FFT(TIME, DATA):
    Fs = len(TIME) / TIME[-1]
    Ts = 1.0 / Fs
    n = len(DATA)
    k = np.arange(n)
    T = n/Fs
    frq = k/T
    frq = frq[range(int(n/2))]
    Y = np.fft.fft(DATA)/n # fft computing and normalization
    Y = Y[range(int(n/2))]
    return frq, Y

frq, Y = FFT(ts, data)
frq2, Y2 = FFT(ts2, data2)
frq3, Y3 = FFT(ts3, data3)
frq4, Y4 = FFT(ts4, data4)

# # SIGNAL A
# fig1, (ax1, ax2) = plt.subplots(2, 1)
# fig1.suptitle("Signal A")
# ax1.plot(ts,data,lw=0.5)
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq,abs(Y),lw=0.5) # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')

# # SIGNAL B
# fig2, (ax1, ax2) = plt.subplots(2, 1)
# fig2.suptitle("Signal B")
# ax1.plot(ts2,data2,'b',lw=0.5)
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq2,abs(Y2),'b',lw=0.5) # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')

# # SIGNAL C
# fig3, (ax1, ax2) = plt.subplots(2, 1)
# fig3.suptitle("Signal C")
# ax1.plot(ts3,data3,'b',lw=0.5)
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq3,abs(Y3),'b',lw=0.5) # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')

# # SIGNAL D
# fig4, (ax1, ax2) = plt.subplots(2, 1)
# fig4.suptitle("Signal D")
# ax1.plot(ts4,data4,'b',lw=0.5)
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq4,abs(Y4),'b',lw=0.5) # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')



##############
### PART 5 ###
##############

def lpf(BUF_SIZE, TIME, DATA):
    data_lpf = []
    ts_lpf = TIME[BUF_SIZE: len(TIME)]
    for i in range (BUF_SIZE, len(DATA)):
        data_lpf.append(sum(DATA[i - BUF_SIZE: i]) / BUF_SIZE)

    return ts_lpf, data_lpf

ts_lpf, data_lpf = lpf(325, ts, data)
frq_lpf, Y_lpf = FFT(ts_lpf, data_lpf)

ts2_lpf, data2_lpf = lpf(350, ts2, data2)
frq2_lpf, Y2_lpf = FFT(ts2_lpf, data2_lpf)

ts3_lpf, data3_lpf = lpf(1, ts3, data3)
frq3_lpf, Y3_lpf = FFT(ts3_lpf, data3_lpf)

ts4_lpf, data4_lpf = lpf(100, ts4, data4)
frq4_lpf, Y4_lpf = FFT(ts4_lpf, data4_lpf)

# SIGNAL A
# plt.figure()
# plt.loglog(frq, abs(Y), lw=0.5, color ="black")
# plt.loglog(frq_lpf, abs(Y_lpf), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal A: LPF with 325 datapoints")

# # SIGNAL B
# plt.figure()
# plt.loglog(frq2, abs(Y2), lw=0.5, color ="black")
# plt.loglog(frq2_lpf, abs(Y2_lpf), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal B: LPF with 350 datapoints")

# # SIGNAL C
# plt.figure()
# plt.loglog(frq3, abs(Y3), lw=0.5, color ="black")
# plt.loglog(frq3_lpf, abs(Y3_lpf), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal C: LPF with 1 datapoints")

# # SIGNAL D
# plt.figure()
# plt.loglog(frq4, abs(Y4), lw=0.5, color ="black")
# plt.loglog(frq4_lpf, abs(Y4_lpf), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal D: LPF with 100 datapoints")



##############
### PART 6 ###
##############

def IIR(A, TIME, DATA):
    B = 1 - A
    data_iir = [0]
    ts_iir = TIME
    for i in range(len(DATA)):
        data_iir.append(A*data_iir[i - 1] + B*DATA[i])

    data_iir.pop(0)
    return ts_iir, data_iir 

ts_iir, data_iir = IIR(0.9, ts, data)
frq_iir, Y_iir = FFT(ts_iir, data_iir)

ts2_iir, data2_iir = IIR(0.95, ts2, data2)
frq2_iir, Y2_iir = FFT(ts2_iir, data2_iir)

ts3_iir, data3_iir = IIR(0.975, ts3, data3)
frq3_iir, Y3_iir = FFT(ts3_iir, data3_iir)

ts4_iir, data4_iir = IIR(0.9, ts4, data4)
frq4_iir, Y4_iir = FFT(ts4_iir, data4_iir)

# # SIGNAL A
# plt.figure()
# plt.loglog(frq, abs(Y), lw=0.5, color ="black")
# plt.loglog(frq_iir, abs(Y_iir), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal A: IRR with a = 0.9; b = 0.1")

# # SIGNAL B
# plt.figure()
# plt.loglog(frq2, abs(Y2), lw=0.5, color ="black")
# plt.loglog(frq2_iir, abs(Y2_iir), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal B: IRR with a = 0.95; b = 0.05")

# # SIGNAL C
# plt.figure()
# plt.loglog(frq3, abs(Y3), lw=0.5, color ="black")
# plt.loglog(frq3_iir, abs(Y3_iir), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal C: IRR with a = 0.975; b = 0.025")

# # SIGNAL D
# plt.figure()
# plt.loglog(frq4, abs(Y4), lw=0.5, color ="black")
# plt.loglog(frq4_iir, abs(Y4_iir), lw=0.5, color = "red")
# plt.xlabel('Freq (Hz)')
# plt.ylabel('|Y(freq)|')
# plt.title("Signal D: IRR with a = 0.9; b = 0.1")



##############
### PART 7 ###
##############

# Example code, computes the coefficients of a low-pass windowed-sinc filter.

fS = 10000  # Sampling rate.
fL = 100  # Cutoff frequency.
N = 91  # Filter length, must be odd.
h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))
h /= np.sum(h)

data_fir = np.convolve(data, h)
ts_fir = np.arange(0, len(data_fir)/fS, 1/fS)
frq_fir, Y_fir = FFT(ts_fir, data_fir)


# SIGNAL A
plt.figure()
plt.loglog(frq, abs(Y), lw=0.5, color ="black")
plt.loglog(frq_fir, abs(Y_fir), lw=0.5, color = "red")
plt.xlabel('Freq (Hz)')
plt.ylabel('|Y(freq)|')
plt.title("Signal A: 100Hz cutoff; 100Hz Bandwidth; Rectangular Filter; 91 coefficients")

plt.figure()
plt.plot(ts, data, lw=0.5, color ="black")
plt.plot(ts_fir, data_fir, lw=0.5, color = "red")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Signal A: 100Hz cutoff; 100Hz Bandwidth; Rectangular Filter; 91 coefficients")

plt.show()

