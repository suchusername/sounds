#pragma once
#include "../server.h"

#include <string>
using namespace std;

AudioFile::AudioFile(string file_id, int size, int fd, int *data, int NumSamples, int SampleRate) : file_id(file_id), size(size), fd(fd), data(data), NumSamples(NumSamples), SampleRate(SampleRate) {}