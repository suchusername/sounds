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
#include "Classes/Queries/Speed.cc"
#include "Classes/Queries/Merge.cc"
#include "Classes/Queries/BitSampleRate.cc"

#include <iostream>
using namespace std;

string sounds_crop(const string &name, const string &new_name, double left, double right) {
	try {
		//cout << name << " " << new_name << endl;
			
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, DEFAULT_FCODE);
		
		int l = (int) (left / 1000 * A.SampleRate) * A.NumChannels;
		int r = (int) (right / 1000 * A.SampleRate) * A.NumChannels;
		
		Crop Q("", l, r);
		Q.transform(&A, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	return "OK";
}

string sounds_volume(const string &name, const string &new_name, double k, double left, double right, bool smooth) {
	try {
		
		
		//php_printf("sounds_volume(%s, %s, %lf, %lf, %lf, %d)\n", name.c_str(), new_name.c_str(), k, left, right, (int) smooth);
		
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, DEFAULT_FCODE);
		
		int l = (int) (left / 1000 * A.SampleRate) * A.NumChannels;
		int r = (int) (right / 1000 * A.SampleRate) * A.NumChannels;
		
		Volume Q("", k, l, r, smooth);
		Q.transform(&A, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	return "OK";
}

string sounds_speed(const string &name, const string &new_name, double mult) {
	try {
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, DEFAULT_FCODE);
		
		Speed Q("", mult);
		Q.transform(&A, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	//php_printf("mult = %f<br>", mult);
	
	return "OK";
}


string sounds_merge(const string &left, const string &right, const string &new_name) {
	try {
		bytevector bl, br;
		bl.read_from_file(left);
		br.read_from_file(right);
		WAV_File L, R;
		
		L.init(bl, DEFAULT_FCODE);
		R.init(br, DEFAULT_FCODE_2);
		
		Merge Q("", &R);
		Q.transform(&L, new_name);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}
	
	return "OK";
}


vector<double> sounds_info(const string &name) {
	
	/*string ret;
	
	try {
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b);
		
		ret = "<br> <br> Stats: <br>";
		ret += "Size: " + to_string(A.size) + " bytes<br>";
		ret += "Number of audio channels: " + to_string(A.NumChannels) + "<br>";
		ret += "Sample rate: " + to_string(A.SampleRate) + " samples/sec<br>";
		ret += "Bit depth: " + to_string(A.BitDepth) + " bits/sample<br>";
		ret += "Number of samples: " + to_string(A.NumSamples) + " samples<br>";
		ret += "Duration: " + to_string((double) A.NumSamples / A.SampleRate) + " sec<br>";
		ret += "<br>";
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		return error;
	}*/
	
	vector<double> ret(6);
	ret[0] = -1;
	
	try {
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b);
		
		ret[0] = (double) A.size;
		ret[1] = (double) A.NumChannels;
		ret[2] = (double) A.SampleRate;
		ret[3] = (double) A.BitDepth;
		ret[4] = (double) A.NumSamples;
		ret[5] = (double) A.NumSamples / A.SampleRate; // duration in seconds
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		//php_printf("error: %s<br>", error.c_str());
		//return error;
	}
	
	return ret;
	
	// if ret[0] == -1, then error
}

vector<double> sounds_classify(const string &name) {
	
	vector<double> ret(1);
	ret[0] = -1;
	
	try {
		
		bytevector b;
		b.read_from_file(name);
		WAV_File A;
		A.init(b, name);
		
		
		ret = A.classify();
		
		//php_printf("... %f %f %f %f %f<br>", ret[0], ret[1], ret[2], ret[3], ret[4]);
		
	} catch (const char *err) {
		printf("%s\n", err);
		string error(err);
		//return error;
	}

	return ret;
}


