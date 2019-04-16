#include <stdio.h>
#include <string>
#include <vector>

#include "server.h"
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"
#include "Classes/Queries/Crop.cc"
#include "Classes/Queries/Volume.cc"
#include "Classes/Queries/BitSampleRate.cc"

#include <iostream>
#include <fstream>
using namespace std;

string sounds_crop(const string &name, const string &new_name, int left, int right) {
	try {
			
		//cout << name << " " << new_name << endl;
			
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, new_name);
		
		Crop Q("", left, right);
		Q.transform(&A, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	return "OK";
}

string sounds_volume(const string &name, const string &new_name, double k, int left, int right, bool smooth) {
	try {
		
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, new_name);
		
		Volume Q("", k, left, right, smooth);
		Q.transform(&A, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	return "OK";
}

string sounds_info(const string &name) {
	try {
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b);
		
		string ret = "<br> <br> Stats: <br>";
		ret += "Size: " + to_string(A.size) + " bytes<br>";
		ret += "Number of audio channels: " + to_string(A.NumChannels) + "<br>";
		ret += "Sample rate: " + to_string(A.SampleRate) + " samples/sec<br>";
		ret += "Bit depth: " + to_string(A.BitDepth) + " bits/sample<br>";
		ret += "Number of samples: " + to_string(A.NumSamples) + " samples<br>";
		ret += "Duration: " + to_string((double) A.NumSamples / A.SampleRate) + " sec<br>";
		ret += "<br>";
		return ret;
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
}




