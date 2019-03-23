#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include "config.h"
#include "params.cc"
using namespace std;

long FdGetFileSize(int fd) {
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int readStringFromFile(int *data, int pos, int len, string &buf, int fileSize) { // len - length of a string to read
	int blocks = (len + 3) / 4; // == # of blocks to read, 1 block = 4 bytes (int)
	buf = "";
	if (fileSize < pos + blocks - 1) return 1; // file is too short
	int j = 0;
	while(j < blocks) {
		int a = data[pos+j];
		int b = a % (1 << 24); // first byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 1)) break; 
		b = (a >> 8) % (1 << 16); // second byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 2)) break; 
		b = (a >> 16) % (1 << 8); // third byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 3)) break; 
		b = a >> 24; // last byte
		buf += (char) b;
		j++;
	}
	return 0;
}

enum Format {
	WAV,
	MP3
};

class SoundFile_WAV {
private:
	// file description as per http://soundfile.sapp.org/doc/WaveFormat/
	string name; //name of file
	int size; //file size
	int *data; //pointer to mmap object
	int fd; //file descriptor

	int Subchunk1Size; // 16 for PCM. This is the size of the rest of the Subchunk which follows this number
	int AudioFormat; // PCM = 1 (i.e. linear quantization)
	int NumChannels; // Mono - 1, Stereo - 2
	int SampleRate; // usually 44100
	int ByteRate; // == SampleRate * NumChannels * BitsPerSample/8
	int BlockAlign; // == NumChannels * BitsPerSample/8
	int BitDepth; // a.k.a. # bits per sample
	int NumSamples; // a.k.a. Subchunk2Size
	// if PCM then extra parameters do not exists and total data offset from the beginning of the file is 44 bytes (or 11 blocks)
	//The default byte ordering assumed for WAVE data files is little-endian. Files written using the big-endian byte ordering scheme have the identifier RIFX instead of RIFF.
	//The sample data must end on an even byte boundary. Whatever that means.
	//8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.
	//There may be additional subchunks in a Wave data stream. If so, each will have a char[4] SubChunkID, and unsigned long SubChunkSize, and SubChunkSize amount of data.
	//RIFF stands for Resource Interchange File Format.
	
public:
	SoundFile_WAV(int *data, int size, int fd, const string &name) { 
		this->name = name;
		this->size = size;
		this->fd = fd;
		this->data = data;
		
		string fileType, fileFormat;
		int err = readStringFromFile(data, 0, 4, fileType, size);
		if (err) throw "Parsing error: unable to read header";
		
		err = readStringFromFile(data, 2, 4, fileFormat, size);
		if (err) throw "Parsing error: unable to read header";
		
		if ((fileType != "RIFF") && (fileFormat != "WAVE")) {
			throw "Parsing error: format is not RIFF WAVE (bytes #0 and #2)";
		}
		
		if (size < 44) {
			throw "Parsing error: file is too short";
		}
		
		if (data[1]+8 != size) {
			throw "Parsing error: incorrect file size in WAV file in byte #1";
		}
		
		string buf;
		readStringFromFile(data, 3, 4, buf, size); // here size is guaranteed to be correct
		if (buf != "fmt ") throw "Parsing error: unable to read header at byte #3";
		
		this->Subchunk1Size = data[4];
		this->AudioFormat = data[5] % (1 << 16);
		if (AudioFormat != 1) {
			throw "Parsing error: audio format is not PCM (Pulse-code modulation) at byte #5";
		}
		this->NumChannels = data[5] >> 16;
		this->SampleRate = data[6];
		this->ByteRate = data[7];
		this->BlockAlign = data[8] % (1 << 16);
		this->BitDepth = data[8] >> 16;
		readStringFromFile(data, 9, 4, buf, size);
		if (buf != "data") throw "Parsing error: audio format is not PCM (Pulse-code modulation) at byte #9";
		this->NumSamples = 8 * data[10] / NumChannels / BitDepth;	
	}
	
	~SoundFile_WAV() {
		if (munmap(data, size) == -1) {
			cout << "Error unmapping the file" << endl;
		}
		close(fd);
	}
	
	void printInfo() {
		cout << "file name: " << name << endl;
		cout << "size: " << size << " bytes" << endl;
		cout << "Subchunk1Size: " << Subchunk1Size << endl;
		cout << "AudioFormat: " << AudioFormat << endl;
		cout << "NumChannels: " << NumChannels << endl;
		cout << "SampleRate: " << SampleRate << endl;
		cout << "ByteRate: " << ByteRate << endl;
		cout << "BlockAlign: " << BlockAlign << endl;
		cout << "BitDepth: " << BitDepth << endl;
		cout << "NumSamples: " << NumSamples << endl;
	}
	
	int getSamplingFreq() {
		return SampleRate;
	}
	
	int getNumSamples() {
		return NumSamples;
	}
	
	int write_csv_file(int limit1 = 0, int limit2 = 0) {
		// if limits == 0 then the whole audio gets written to csv file
		// if limit != 0 then only first limit1 samples get written
		// if both limits != 0 then samples from range [limit1, limit) are written
		
		// SAMPLES_PER_LINE samples per line (32 if there are 2 audio channels, samples for different channels alternate in this case)
		// Ex.: 0,0,1,1,2,3,4,...
		//cout << (data[11] >> 16) << endl;
		//cout << data[11] % (1 << 16) << endl;
		int fd;
		fd = open("sound.csv", O_RDWR | O_TRUNC | O_CREAT | O_NONBLOCK, 0666);
		if (fd < 0) {
			cout << "Failed to open a file" << endl;
			return 1; 
		}
		NumSamples = NumSamples / 2 * 2; // making sure it is an even number
		int a = 0;
		int b = NumSamples / 2 * NumChannels;
		if (limit1 > 0) {
			b = limit1 / 2 * NumChannels;
		}
		if (limit2 > 0) {
			a = limit1 / 2 * NumChannels;
			b = limit2 / 2 * NumChannels;
		}
		for (int i = a; i < b; i++) {
			if (((i) % (SAMPLES_PER_LINE / 2 * NumChannels) == 0) && (i > a)) {
				write(fd, "\n", 1);
			}	
			string buf = to_string((short) (data[11+i] % (1 << 16))) + ".,";
			write(fd, buf.c_str(), buf.length());
			buf = to_string((short) (data[11+i] >> 16)) + ".";
			if ((i+1) % (SAMPLES_PER_LINE / 2 * NumChannels) != 0) {
				buf += ",";
			}
			write(fd, buf.c_str(), buf.length());		
			// offset (in 4bytes) = 11
		}
		
		// adding 0's at the end to make same number of columns
		int c = SAMPLES_PER_LINE - (((b - a) * 2 / NumChannels) % (SAMPLES_PER_LINE));
		if (c == 16) c = 0;
		for (int i = 0; i < c; i++) {
			if (i < c-1) {
				write(fd, "0.,", 3);
			} else {
				write(fd, "0.", 2);
			}
		}
		
		return 0;
	}
	
	void debug() {
		cout << (short) (data[136] % (1 << 16)) << endl;
	}
};

int main(int argc, char *argv[]) {
	/* Format needs to be detected automatically */
	// arg1 = format
	// arg2 = samples_to_show
	// arg3 = right_border (if no arg4) & left_border (if arg4)
	// arg4 = right_border
	
	ArgMap args;
	try {
		args.fill(argc, argv);
	} catch(const char *str) {
		write(2, str, strlen(str));
		return 0;
	}
	
	Format format = WAV; 
	
	/* Redirecting strerr to a file */
	close(2);
	int fderr = open("logs.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fderr < 0) perror("open");
	
	int fd;
	int *data;
	fd = open(args.get((char *)"file").c_str(), O_RDONLY);
	if (fd < 0) {
		cout << "Failed to open a file" << endl;
		return 0; 
	}
	int fileSize = FdGetFileSize(fd);
	if (fileSize < 0) {
		cout << "Failed to get file size" << endl;
		return 0;
	}
	//cout << fileSize << endl;
	data = (int *)mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);
	if (data == MAP_FAILED) {
		close(fd);
		cout << "Error mmapping the file" << endl;
		return 0;
	}
	
	try {
		switch (format) {
			case WAV: {
				
				SoundFile_WAV F(data, fileSize, fd, args.get((char *)"file")); // class object
				F.printInfo();
				int err = F.write_csv_file();
				//F.debug();
				
				string _text;
				if (err) {
					_text = "ERROR: WAVparse.cc: Error while writing a sound.csv file.\n";
				} else {
					_text = "WAVparce.cc: sound.csv file written successfully.\n";
				}
				write(2, _text.c_str(), _text.length());
				
				int pid = fork();
				if (pid < 0) perror("fork");
				if (pid == 0) { // child calling fft.py
					
					string sampling_freq = to_string(F.getSamplingFreq());
					string num_samples = to_string(F.getNumSamples());
					/*string samples_to_show = SAMPLES_TO_SHOW_DFL_RIGHT;
					string left_border = LEFT_FREQ_BORDER_DFL;
					string right_border = RIGHT_FREQ_BORDER_DFL;
					if (argc > 2) {
						samples_to_show.assign(argv[2]);
					}
					if (argc > 3) {
						right_border.assign(argv[3]);
					}
					if (argc > 4) {
						left_border.assign(argv[3]);
						right_border.assign(argv[4]);
					}*/
					
					vector<char *> caller; // vector with argument to call fft.py
					caller.push_back((char *)"python3");
					caller.push_back((char *)FFT_FILE_NAME.c_str());
					caller.push_back((char *)sampling_freq.c_str());
					caller.push_back((char *)num_samples.c_str());
					caller.push_back((char *)args.get((char *)"samples_left").c_str());
					caller.push_back((char *)args.get((char *)"samples_right").c_str());
					caller.push_back((char *)args.get((char *)"freq_left").c_str());
					caller.push_back((char *)args.get((char *)"freq_right").c_str());
					caller.push_back(NULL);
					
					//close(2);
					if (execvp(caller[0], &caller[0]) < 0) {
						_text = "ERROR: WAVparse.cc: error while calling fft.py.\n";
						write(2, _text.c_str(), _text.length());
						close(2);
						_exit(0);
					}
					
				} else {
					wait(0);
				}
				
				_text = "WAVparce.cc: processing completed.\n";
				write(2, _text.c_str(), _text.length());
							
				break;
			}
			case MP3: {
				cout << "MP3 case..." << endl;
				break;
			}
		}
	} catch(const char *str) {
		write(2, str, strlen(str));
	}
	
	close(2)
	return 0;
}