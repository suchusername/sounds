#include <stdio.h>
#include <string>

#include "wrapper.h"

#include <iostream>
using namespace std;

int main() {
	
	sounds_crop("Audios/alltta_bucket.wav", "result", 1000000, -1);
	sounds_volume("Audios/Archive/result.wav", "result", 0.05, 200000, 1800000, 1);
	
	//cout << sounds_hi("hihi") << endl;
	
	/*try {
		WAV_File A;
		A.load("Audios/alltta_bucket.wav");
		A.print();
		
		Crop X("", 0, 2000000);
		X.transform(&A, "hahahha");
		
		Volume Y("", 5, 200000, 1800000, 1);
		Y.print();
		Y.transform(&A, "hahahha");
		
	} catch(const char *err) {
		printf("%s\n", err);
	}*/
}

