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
using namespace std;

int main() {
	
	try {
		WAV_File A;
		A.load("Audios/Archive/alim_A.wav");
		A.print();
		
		A.classify();
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}

