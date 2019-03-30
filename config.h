#pragma once
#include <string>

#define CONFIG_H

const std::string FILE_SAVE_DIRECTORY = "Audios/Archive"; // path from root of directory, used by WAV_File::init()
const std::string DEFAULT_FCODE = "123234782374"; // TODO: remove, used by WAV_File::init()

const std::string DEFAULT_FILE_NAME = "Audios/tone1sec.wav";
const int SAMPLES_PER_LINE = 16;

const std::string FFT_FILE_NAME = "fft.py";
const std::string SAMPLES_TO_SHOW_DFL_LEFT = "0";
const std::string SAMPLES_TO_SHOW_DFL_RIGHT = "2000";
const std::string LEFT_FREQ_BORDER_DFL = "-1";
const std::string RIGHT_FREQ_BORDER_DFL = "500";