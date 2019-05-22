#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <string>
#include <iostream>
using namespace std;

Merge::Merge() : Query(), right(NULL) {}

Merge::Merge(const string &s, WAV_File *p) : Query(s), right(p) {}

int Merge::init(bytevector const &v, const string &s) {
	throw "Merge::init(): not implemented.";
	return 0;
}

bytevector Merge::serialize() const {
	throw "Merge::serialize(): not implemented.";
	bytevector b;
	return b;
}

void Merge::print() const {
	cout << "file_id = " << file_id << endl;
}

void Merge::transform(WAV_File *left, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	
	int chan = 1;
	if ((left->NumChannels == 2) || (right->NumChannels == 2)) chan = 2;
	
	UniformDataSamples arr_left = left->getSamples(chan);	
	UniformDataSamples arr_right = right->getSamples(chan);
	
	UniformDataSamples arr(arr_left.N + arr_right.N);
	
	for (int i = 0; i < arr_left.N; i++) {
		arr[i] = arr_left[i];
	}
	
	
	for (int i = 0; i < arr_right.N; i++) {
		arr[arr_left.N + i] = arr_right[i];
	}
	
	WAV_File *file = left;
	
	bytevector b(44 + 2*arr.N); 
	b.writeString("RIFF", 0);
	b.writeInt(36+2*arr.N, 4); // new size
	b.writeString("WAVE", 8);
	b.writeString("fmt ", 12);
	b.writeInt(file->Subchunk1Size, 16);
	writeShort(file->AudioFormat, b, 20);
	writeShort(chan, b, 22); // NumChannels
	b.writeInt(file->SampleRate, 24);
	b.writeInt(file->ByteRate, 28);
	writeShort(chan * file->BitDepth / 8, b, 32); // new BlockAlign
	writeShort(file->BitDepth, b, 34);
	b.writeString("data", 36);
	b.writeInt(2*arr.N, 40);
	
	for (int i = 0; i < arr.N; i++) writeShort((int) arr[i], b, 44+2*i);
	
	//php_printf("bytevector size = %d<br>", b.size);
	
	file->init(b, new_id);	
}




