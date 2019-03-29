#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/DataSamples.cc"
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"

int main() {	
	try {		
		bytevector v;
		string file_name = "Audios/o_10sec.wav";
		v.read_from_file(file_name);
		
		WAV_File X;
		X.init(v);
		X.print();
		
		bytevector w = X.serialize();
		
		WAV_File Y;
		Y.init(w);
		Y.print();
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
}