import numpy as np
import wave
from pydub import AudioSegment
import random
import sys
import io
import os
import glob
import IPython

import re
import sys

import numpy as np
from keras import layers
from keras.layers import Input, Dense, Activation, ZeroPadding2D, BatchNormalization, Flatten, Conv2D
from keras.layers import AveragePooling2D, MaxPooling2D, Dropout, GlobalMaxPooling2D, GlobalAveragePooling2D, \
    GlobalMaxPooling3D
from keras.models import Model
from keras.preprocessing import image
from keras.utils import layer_utils
from keras.utils.data_utils import get_file
from keras.applications.imagenet_utils import preprocess_input
from keras.callbacks import ModelCheckpoint
import pydot
from IPython.display import SVG
from keras.utils.vis_utils import model_to_dot
from keras.utils import plot_model
from keras.models import load_model
# from kt_utils import *

import keras.backend as K

K.set_image_data_format('channels_last')
import matplotlib.pyplot as plt
from matplotlib.pyplot import imshow



window = 0.2  # sec
file = sys.argv[1]
# file = 'piano_4.wav'


import matplotlib.pyplot as plt
from scipy.io import wavfile
import os
from pydub import AudioSegment


# Calculate and plot spectrogram for a wav audio file
def my_graph_spectrogram(rate, data):
    # rate, data = get_wav_info(wav_file)
    nfft = 200  # Length of each window segment
    fs = 8000  # Sampling frequencies
    noverlap = 120  # Overlap between windows
    nchannels = data.ndim
    if nchannels == 1:
        pxx, freqs, bins, im = plt.specgram(data, nfft, fs, noverlap=noverlap)
    elif nchannels == 2:
        pxx, freqs, bins, im = plt.specgram(data[:, 0], nfft, fs, noverlap=noverlap)
    return pxx


def graph_spectrogram(wav_file):
    rate, data = get_wav_info(wav_file)
    nfft = 200  # Length of each window segment
    fs = 8000  # Sampling frequencies
    noverlap = 120  # Overlap between windows
    nchannels = data.ndim
    if nchannels == 1:
        pxx, freqs, bins, im = plt.specgram(data, nfft, fs, noverlap=noverlap)
    elif nchannels == 2:
        pxx, freqs, bins, im = plt.specgram(data[:, 0], nfft, fs, noverlap=noverlap)
    return pxx


# Load a wav file
def get_wav_info(wav_file):
    rate, data = wavfile.read(wav_file)
    return rate, data


# Used to standardize volume of audio clip
def match_target_amplitude(sound, target_dBFS):
    change_in_dBFS = target_dBFS - sound.dBFS
    return sound.apply_gain(change_in_dBFS)


def get_duration_wav(wav_filename):
    f = wave.open(wav_filename, 'r')
    frames = f.getnframes()
    rate = f.getframerate()
    duration = frames / float(rate)
    f.close()
    return duration


def most_frequent(List):
    return max(set(List), key=List.count)


def what_is_it(Y):
    list_ = []
    for row in Y:
        list_.append(np.argmax(row))
    res = most_frequent(list_)
    if (res == 0):
        print('accordion')
    elif (res == 1):
        print('piano')
    elif (res == 2):
        print('violin')
    elif (res == 3):
        print('guitar')
    else:
        print('noise')


dur = get_duration_wav(file)

X = []

print(file)
rate, data = get_wav_info(file)
if (data.ndim > 1):
    data = np.delete(data, 1, 1)
    # print(data.shape)
a = int((1 // window) * 10)
b = int((1 // window) * dur) - 10
# print(a,b)

for s in range(a, b, 20):
    data_ = data
    t1 = s * window
    t2 = t1 + window
    t1 = int(t1 * 44100)  # Works in milliseconds
    t2 = int(t2 * 44100)
    # print(t1, t2)
    data_ = data_[t1:t2]
    x = my_graph_spectrogram(rate, data_)
    X.append(x)

X = np.array(X)
model = load_model('my_model.h5')
maxElement = 16462066.603519555
X = X / maxElement

train_shape = list(X.shape)
train_shape.append(1)
X = X.reshape(train_shape)
# print(X.shape)

Y = model.predict(X)


print('\n')
print('\n')
print('\n')
print('\n')
print('\n')
print('\n')

print('And it is most likely to be...')
what_is_it(Y)