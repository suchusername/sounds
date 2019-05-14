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

#include <iostream>
using namespace std;

int main() {
	
	try {
		WAV_File A;
		A.load("../Audios/Archive/bayan.wav");
		//A.print();
		
		/*Crop C("", 0, -1);
		C.transform(&A, "Audios/Archive/bayanLOUD.wav");
		
		Volume V("", 0);
		//V.print();
		V.transform(&A, "Audios/Archive/bayanLOUD.wav");*/
		
		cout << A.classify() << endl;
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}

