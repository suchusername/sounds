#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"
#include "Classes/Crop.cc"

#include <iostream>
using namespace std;

int main() {	
	try {
		bytevector b;
		b.read_from_file("Audios/alltta_bucket.wav");
		WAV_File A;
		A.init(b);
		A.print();
		
		Crop C("3213", 0, 5000000);
		C.transform(&A, "8002130");
		A.print();
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}