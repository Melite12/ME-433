import serial
import matplotlib.pyplot as plt
import numpy as np


PORT = "COM7"
BAUD = 115200
NUM_SAMPLES = 500

ser = serial.Serial(PORT, BAUD, timeout=5)

# Request a batch
ser.reset_input_buffer()
ser.write(f"{NUM_SAMPLES}\n".encode())

times = []
values = []

for _ in range(NUM_SAMPLES):
    try:
        raw = ser.readline().decode(errors="replace").strip()
        parts = raw.split()
        if len(parts) == 3:
            _idx, t, v = int(parts[0]), int(parts[1]), int(parts[2])
            times.append(t)
            values.append(v)
    except ValueError:
        continue

time1 = times[0]
for i in range(len(times)):
    times[i] = (times[i] - time1)/1000

ser.close()


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

frq, Y = FFT(times, values)

def IIR(A, TIME, DATA):
    B = 1 - A
    data_iir = [0]
    ts_iir = TIME
    for i in range(len(DATA)):
        data_iir.append(A*data_iir[i - 1] + B*DATA[i])

    data_iir.pop(0)
    return ts_iir, data_iir 


ts_iir, data_iir = IIR(0.1, times, values)
frq_iir, Y_iir = FFT(ts_iir, data_iir)


### RAW DATA ###
fig1, (ax1, ax2) = plt.subplots(2, 1)
fig1.suptitle(f"Raw Data: {NUM_SAMPLES} datapoints")
ax1.plot(times,values,lw=0.5)
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')

### Filtered Data ###
fig2, (ax1, ax2) = plt.subplots(2, 1)
fig2.suptitle(f"Filtered Data (a = 0.1): : {NUM_SAMPLES} datapoints")
ax1.plot(ts_iir,data_iir,lw=0.5)
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq_iir,abs(Y_iir),lw=0.5) # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')


plt.show()