#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <math.h>
#include <string>
#include <iostream>
using namespace std;

Speed::Speed() : Query(), mult(1) {}

Speed::Speed(const string &s, double m) : Query(s), mult(m) {}

int Speed::init(bytevector const &v, const string &s) {
	throw "Speed::init(): not implemented.";
	return 0;
}

bytevector Speed::serialize() const {
	throw "Speed::serialize(): not implemented.";
	bytevector b;
	return b;
}

void Speed::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "mult = " << mult << endl;
}

void Speed::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	
	UniformDataSamples old_arr = file->getSamples();
	
	
	if (mult == 0) throw "Speed::transform(): Speed multiplier cannot be equal to 0.";
	
	//php_printf("%f<br>", mult);
	
	double new_mult = mult;
	int len = old_arr.N;
	if (mult < 0) { // reversing an array
		for (int i = 0; i < len/2; i++) {
			double t = old_arr[i];
			old_arr[i] = old_arr[len-1-i];
			old_arr[len-1-i] = t;
		}
		new_mult *= -1;
	} 
	
	if (new_mult == 2) new_mult += 0.0001;
	old_arr.delta_x = 1 / new_mult; // converting to points from seconds and speeding up
	// now points are not whole numbers, needs interpolation
	
	int new_len = floor(old_arr.delta_x * (len - 1)) + 1;
	
	php_printf("new_len = %d, len = %d<br>", new_len, len);
	
	UniformDataSamples arr(new_len);
	arr.x_0 = 0;
	arr.delta_x = old_arr.delta_x / file->SampleRate; // converting back to seconds
	
	for (int i = 0; i < len - 1; i++) { 
		int l = ceil(i * old_arr.delta_x);
		int r = floor((i+1) * old_arr.delta_x);
		for (int j = l; j <= r; j++) { // going over all integers between 2 points
			double alpha = ((double) j - old_arr.delta_x * i) / old_arr.delta_x;
			arr[j] = round((1 - alpha) * old_arr[i] + alpha * old_arr[i+1]);
		}
	}
	
	/*int S = 0;
	for (int i = 0; i < arr.N; i++) {
		S += (old_arr[i] - arr[i]);
	}
	cout << S << endl;*/
	
	bytevector b(44 + 2*arr.N); 
	b.writeString("RIFF", 0);
	b.writeInt(36+2*arr.N, 4); // new size
	b.writeString("WAVE", 8);
	b.writeString("fmt ", 12);
	b.writeInt(file->Subchunk1Size, 16);
	writeShort(file->AudioFormat, b, 20);
	writeShort(1, b, 22); // NumChannels
	b.writeInt(file->SampleRate, 24);
	b.writeInt(file->ByteRate, 28);
	writeShort(file->BlockAlign, b, 32);
	writeShort(file->BitDepth, b, 34);
	b.writeString("data", 36);
	b.writeInt(2*arr.N, 40);
	
	for (int i = 0; i < arr.N; i++) writeShort((int) arr[i], b, 44+2*i);
	
	php_printf("bytevector size = %d<br>", b.size);
	
	file->init(b, new_id);	
}




