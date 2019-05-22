#pragma once
#include "../../server.h"
#include "../../config.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <math.h>
#include <string>
#include <iostream>
using namespace std;

void Volume::writeInt(int x, bytevector &b, int pos) const {
	int x_ = x + (1 << 30);
	for (int i = 0; i < 4; i++) {
		b[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}
int Volume::readInt(bytevector const &b, int pos) const {
	int N_ = 0;
	for (int i = 0; i < 4; i++) {
		N_ += ((256 + (int) b[pos+i]) % 256) << (8*i);
	}
	return N_ - (1 << 30);
}

void Volume::writeDouble(double x, bytevector &b, int pos) const {
	if (pos+7 >= b.size) throw "UniformDataSamples::serialize(): error writing double to bytevector.";
	if ((x >= 1e9) || (x <= 1-1e9)) throw "UniformDataSamples::serialize(): double out of range.";
	int X = floor(x);
	writeInt(X, b, pos);
	int Y = floor((x - X) * 1e9);
	writeInt(Y, b, pos+4);
}

double Volume::readDouble(bytevector const &b, int pos) const {
	double a1 = (double) readInt(b, pos);
	double a2 = (double) readInt(b, pos+4);
	//cout << readInt(b, pos) << " " << readInt(b, pos+4) << endl;
	return a1 + a2 * 1e-9;
}


Volume::Volume() : Query(), k(1) {}

Volume::Volume(const string &s, double k, int l, int r, bool smooth) : Query(s), k(k), left(l), right(r), smooth(smooth) {}

int Volume::init(bytevector const &v, const string &s) {
	throw "Volume::init(): not implemented.";
	return 0;
}

bytevector Volume::serialize() const {
	// redundant
	throw "Volume::serialize(): not implemented.";
	bytevector b;
	return b;
}


void Volume::print() const {
	cout << "--- Volume query ---" << endl;
	cout << "file_id = " << file_id << endl;
	cout << "k = " << k << endl;
	cout << "left = " << left << endl;
	cout << "right = " << right << endl;
	cout << "smooth = " << (int)smooth << endl;
}

void Volume::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	
	UniformDataSamples arr = file->getSamples(file->NumChannels);
	
	int l = left;
	int r = right;
	
	if ((r > arr.N) || (r < 0)) r = arr.N;
	if ((l >= arr.N) || (l < 0)) l = 0;
	if (l > r) {
		l ^= r;
		r ^= l;
		l ^= r;
	}
	
	
	//cout << "l = " << l << ", r = " << r << endl;
	//cout << file->NumSamples << " " << arr.N << endl;
	
	if (smooth) {
		// multiplies a signal by linear function with 2 second fade in / fade out		
		
		int fade = QUERY_VOLUME_DEFAULT_FADE_IN;
		if (2*fade > r - l) {
			fade = (r - l) / 2;
		}
		if (fade == 0) throw "Volume::transform(): fade is equal to 0.";
		
		//cout << l << " " << r << " " << fade << endl;
		
		double alpha = ((double) (k-1)) / ((double) fade);
		double beta = ((double) (1-k)*l) / ((double) fade) + 1;
		
		for (int i = l; i < l+fade; i++) {
			arr[i] = (int) (arr[i] * (alpha * i + beta));
			if (arr[i] > 32767) arr[i] = 32767;
			if (arr[i] < -32767) arr[i] = 32767;
		}
		
		for (int i = l+fade; i < r-fade; i++) {
			arr[i] = (int) (arr[i] * k);
			if (arr[i] > 32767) arr[i] = 32767;
			if (arr[i] < -32767) arr[i] = 32767;
		}
		
		alpha = ((double) (1-k)) / ((double) fade);
		beta = ((double) (k-1)*r) / ((double) fade) + 1;
		
		for (int i = r-fade; i < r; i++) {
			arr[i] = (int) (arr[i] * (alpha * i + beta));
			if (arr[i] > 32767) arr[i] = 32767;
			if (arr[i] < -32767) arr[i] = 32767;
		}
		
	} else {
		//cout << "l = " << l << ", r = " << r << endl;
		for (int i = l; i < r; i++) {
			//if (13500 < i && i < 13510) cout << arr[i] << endl;
			arr[i] = (int) (arr[i] * k);
			if (arr[i] > 32767) arr[i] = 32767;
			if (arr[i] < -32767) arr[i] = 32767;
			//if (10500 < i && i < 13510) cout << arr[i] << endl;
		}
	}
	
		
	bytevector b(44 + 2*arr.N);
	b.writeString("RIFF", 0);
	b.writeInt(36+2*arr.N, 4); // new size
	b.writeString("WAVE", 8);
	b.writeString("fmt ", 12);
	b.writeInt(file->Subchunk1Size, 16);
	writeShort(file->AudioFormat, b, 20);
	writeShort(file->NumChannels, b, 22); // NumChannels
	b.writeInt(file->SampleRate, 24);
	b.writeInt(file->ByteRate, 28);
	writeShort(file->BlockAlign, b, 32);
	writeShort(file->BitDepth, b, 34);
	b.writeString("data", 36);
	b.writeInt(2*arr.N, 40);
	
	//for (int i = 0; i < arr.N; i++) if (10500 < i && i < 13510) cout << arr[i] << endl;
	for (int i = 0; i < arr.N; i++) writeShort((int) arr[i], b, 44+2*i);
	
	file->file_id = "";
	file->init(b, new_id);	
}




