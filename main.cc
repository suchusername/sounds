#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"
#include "Classes/Crop.cc"
#include "Classes/Volume.cc"

#include <iostream>
using namespace std;

int main() {	
	try {
		bytevector b;
		b.read_from_file("Audios/alltta_bucket.wav");
		WAV_File A;
		A.init(b);
		A.print();
		
		Crop C("123", 0, 1600000);
		C.transform(&A, "2130956");
		
		Volume V("2130956", 10);
		V.transform(&A, "2130956");
		A.print();
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}