#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"
#include "Classes/Queries/Crop.cc"
#include "Classes/Queries/Volume.cc"
#include "Classes/Queries/BitSampleRate.cc"

#include <iostream>
using namespace std;

int main() {
	try {
		bytevector b;
		b.read_from_file("Audios/alltta_bucket.wav");
		WAV_File A;
		A.init(b);
		A.print();
		
		Crop X("", 0, 2000000);
		X.transform(&A, "hahahha");
		
		Volume Y("", 0.05, 200000, 1800000, 1);
		Y.print();
		Y.transform(&A, "hahahha");
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}

