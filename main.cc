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
		A.load("Audios/Archive/bayan.wav");
		//A.print();
		
		Crop Q1("", 0, 150000);
		Q1.transform(&A, "Audios/Archive/b1.wav");
		
	 
		WAV_File B,C;
		B.load("Audios/Archive/b1.wav");
		C.load("Audios/Archive/ShepardTone.wav");
		B.print();
		C.print();
		
		Merge M("", &C);
		M.transform(&B, "Audios/Archive/merged.wav");
		B.load("Audios/Archive/merged.wav");
		B.print();
		
		/*Volume V("", 0);
		//V.print();
		V.transform(&A, "Audios/Archive/bayanLOUD.wav");*/
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}

