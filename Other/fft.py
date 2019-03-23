
# coding: utf-8

# In[13]:

import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft
from scipy.signal import blackman
from scipy.signal import blackmanharris
from scipy.signal import hann
from scipy.signal import hamming
from ampd import ampdFast

PLOT_WAVE = False
PLOT_FULL_SPECTRUM = False
PLOT_FULL_LOG_SPECTRUM = False
PLOT_PART_SPECTRUM = False
PLOT_PART_LOG_SPECTRUM = False
PLOT_PART_WINDOWS = True


# run this file as: 
# python3 fft.py 44100 80000 2000 300 600
    # arg1 = sampling_freq
    # arg2 = num_samples
    # arg3 = left sample border
    # arg4 = right sample border
    # arg5 = left border
    # arg6 = right border

argv = sys.argv
argc = len(argv)

if argc < 6:
    sys.stderr.write("ERROR: fft.py: Wrong number of arguments\n")
    sys.stderr.close()
    sys.exit()

sampling_freq =  int(argv[1])
N = int(argv[2]) # number of sample points
L = int(argv[3]) # number of points to be shown
R = int(argv[4])
left = int(argv[5]) # [Hz] left border
right = int(argv[6]) # [Hz] right border

if R < L:
    sys.stderr.write('Warning: fft.py: left and right sample borders swapped')
    L, R = R, L

if left > right:
    sys.stderr.write('Warning: fft.py: left and right frequency borders swapped')
    
if L > N:
    sys.stderr.write('Warning: fft.py: requested number of samples to show was reduced to the total number of samples.\n')
    L = 0

if R > N:
    sys.stderr.write('Warning: fft.py: requested number of samples to show was reduced to the total number of samples.\n')
    R = N

rec_time = (N-1) / sampling_freq

# reading data

data = np.loadtxt("sound.csv", delimiter=",").flatten()
data = np.trim_zeros(data, 'b')

x = np.linspace(L, R-1, R-L)

# plotting the beginning of an audio signal

if PLOT_WAVE:
    fig, ax = plt.subplots(figsize=(20,10))
    ax.plot(x, data[L:R])
    ax.set(title = str(R-L) + " samples of an audio signal",
           xlabel = "Sample number")
    fig.savefig("Visuals/wave.png")


# performing FFT

N_data, = data.shape #number of sample points in data array
T = rec_time / (N-1) #sample spacing
x = np.linspace(0.0, rec_time, N)
data = np.concatenate((data, np.zeros((N-N_data,))))
xf = np.linspace(0, 1.0/(2.0*T), N//2)
mask = (xf > left) * (xf < right)


if PLOT_FULL_SPECTRUM or PLOT_FULL_LOG_SPECTRUM or PLOT_PART_SPECTRUM or PLOT_PART_LOG_SPECTRUM or PLOT_PART_WINDOWS:
    yf = fft(data)
    yf2 = 2.0/N * np.abs(yf[0:N//2]) + 10e-7 # making sure it is not 0
    zf = np.log10(yf2 / 10e-5)



# plotting the full spectrum

if PLOT_FULL_SPECTRUM:
    fig, ax = plt.subplots(figsize=(20,10))
    ax.plot(xf, 2.0/N * np.abs(yf[0:N//2]))
    ax.grid()
    fig.savefig("Visuals/full_spectrum.png")

# plotting full spectrum in logarithmic scale

if PLOT_FULL_LOG_SPECTRUM:
    fig, ax = plt.subplots(figsize=(20,10))
    ax.plot(xf, zf)
    ax.grid()
    fig.savefig("Visuals/full_log_spectrum.png")


# plotting only a part of a spectrum



if PLOT_PART_SPECTRUM:
    fig, ax = plt.subplots(figsize=(20,10))
    ax.plot(xf[mask], yf2[mask])
    ax.grid()
    fig.savefig("Visuals/part_spectrum.png")

# plotting only a part of a log spectrum

if PLOT_PART_LOG_SPECTRUM:
    fig, ax = plt.subplots(figsize=(20,10))
    ax.plot(xf[mask], zf[mask])
    ax.grid()
    fig.savefig("Visuals/part_log_spectrum.png")

# using blackman/hann window function

if PLOT_PART_WINDOWS:
    w = blackman(N)
    ywf = fft(data*w)
    ywf2 = 2.0/N * np.abs(ywf[1:N//2])
    fig, ax = plt.subplots(figsize=(20,10))
    #ax.semilogy(xf[mask], yf2[mask], color='b', label="FFT")
    #ax.semilogy(xf[1:][mask[1:]], ywf2[mask[1:]], color='r', label="FFT w. blackman")

    w = hann(N)
    ywf = fft(data*w)
    ywf2 = 2.0/N * np.abs(ywf[1:N//2])
    #ax.semilogy(xf[1:][mask[1:]], ywf2[mask[1:]], color='g', label="FFT w. hann")

    w = hamming(N)
    ywf = fft(data*w)
    ywf2 = 2.0/N * np.abs(ywf[1:N//2])
    #ax.semilogy(xf[1:][mask[1:]], ywf2[mask[1:]], color='y', label="FFT w. hamming")

    w = blackmanharris(N)
    ywf = fft(data*w)
    ywf2 = 2.0/N * np.abs(ywf[1:N//2])
    ax.semilogy(xf[1:][mask[1:]], ywf2[mask[1:]], color='black', label="FFT w. blackmanharris")

    ax.grid()
    fig.legend(loc="upper right", fontsize = 18)
    fig.savefig("Visuals/part_log_spectrum_windows.png")


# detecting peaks on blackman-harris windowed signal spectrum

w = blackmanharris(N)
ywf = fft(data*w)
ywf2 = 2.0/N * np.abs(ywf[1:N//2]) 

zwf = np.log10((ywf2 + 1e-8) / 10e-5)

peaks = ampdFast(zwf, 350)
peaks = peaks[zwf[peaks].argsort()[-100:][::-1]]

#print(peaks[:100])

fig, ax = plt.subplots(figsize=(20,10))
ax.plot(xf[1:][mask[1:]], ywf2[mask[1:]], color='black', label="FFT w. blackmanharris")
ax.grid()
fig.legend(loc="upper right", fontsize = 18)
fig.savefig("Visuals/blackmanharris.png")

fig, ax = plt.subplots(figsize=(20,10))
ax.plot(xf[1:][mask[1:]], zwf[mask[1:]], color='black', label="log FFT w. blackmanharris")
ax.plot(xf[peaks][mask[peaks]], zwf[peaks][mask[peaks]], 'ro')
ax.grid()
fig.legend(loc="upper right", fontsize = 18)
fig.savefig("Visuals/log_blackmanharris.png")




sys.stderr.write('fft.py: processing completed.\n')
sys.stderr.close()




