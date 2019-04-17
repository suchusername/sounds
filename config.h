#pragma once
#include <string>

#define CONFIG_H

const std::string PATH_TO_AUDIOS_PHP = "..";

const int QUERY_VOLUME_DEFAULT_FADE_IN = 300000; // used by Volume::transform()

const std::string FILE_SAVE_DIRECTORY = "Audios/Archive"; // path from root of directory, used by WAV_File::init()
const std::string DEFAULT_FCODE_LOAD = "Audios/Archive/temp.wav"; // in relation to SoundProject, used by WAV_File::load()
const std::string DEFAULT_FCODE = "../Audios/Archive/temp_php.wav"; // in relation to SoundProject/root, used by WAV_File::init() 

const std::string DEFAULT_FILE_NAME = "Audios/tone1sec.wav";
const int SAMPLES_PER_LINE = 16;

const std::string FFT_FILE_NAME = "fft.py";
const std::string SAMPLES_TO_SHOW_DFL_LEFT = "0";
const std::string SAMPLES_TO_SHOW_DFL_RIGHT = "2000";
const std::string LEFT_FREQ_BORDER_DFL = "-1";
const std::string RIGHT_FREQ_BORDER_DFL = "500";