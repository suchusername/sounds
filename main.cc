#include <stdio.h>
#include <string>
#include <vector>

#include <unistd.h>

#include "server.h"
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"
#include "Classes/Queries/Crop.cc"
#include "Classes/Queries/Volume.cc"
#include "Classes/Queries/BitSampleRate.cc"
#include "Classes/Queries/Speed.cc"
#include "Classes/Queries/Merge.cc"

#include <iostream>
using namespace std;

int main() {
	
	try {
		WAV_File A;
		A.load("Audios/Archive/cleanbass-E-twang01.wav");
		A.print();
		
		Crop C1("", 0, 10000);
		C1.transform(&A, "Audios/Archive/nn.wav");
		
		//WAV_File B,C;
		//B.load("Audios/Archive/short.wav");
		//C.load("Audios/Archive/short.wav");
		
		//Merge M("", &C);
		//M.transform(&B, "Audios/Archive/merged.wav");
		
		/*Volume V("", 0);
		//V.print();
		V.transform(&A, "Audios/Archive/bayanLOUD.wav");*/
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}

