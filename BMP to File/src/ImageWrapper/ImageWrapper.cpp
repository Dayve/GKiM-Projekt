#include "ImageWrapper.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

ImageWrapper::ImageWrapper() :
	loadedFlag(false), AC(&buffer)
{}


ImageWrapper::~ImageWrapper() {}


void ImageWrapper::Load(const string& filename) {
	if(img.loadFromFile(filename)) {
		cout << filename <<" loaded\n";
		loadedFlag = true;
	}
	else cout << "Error while loading image.\n";
}


void ImageWrapper::ExportFile(bool codingType, bool grayscale, const string& dataDir) {
	if(!loadedFlag) return;

	string fullPath = dataDir + "output.file";	// Concatenate strings to obtain path to file
	ofstream outputFile(fullPath.c_str(), ios::binary | ios::out);

	// Fetching from img (sf::Image -> sf::Uint8) to buffer (std::vector<sf::Uint8>):
	for(unsigned int yy=0 ; yy<img.getSize().y ; ++yy) {
		for(unsigned int xx=0 ; xx<img.getSize().x ; ++xx) {
			buffer.push_back(img.getPixel(xx, yy).r);
			buffer.push_back(img.getPixel(xx, yy).g);
			buffer.push_back(img.getPixel(xx, yy).b);
		}
	}

	// fileBuffer is a buffer for writing into a file, revBuffer is only for holding the reversed version of 5ths of bits
	vector<bool> fileBuffer, revBuffer;		

	for(int i=0 ; i<buffer.size() ; ++i) {
		sf::Uint8 scaledVal = (buffer[i] * (pow(2, NR_BITS)-1))/255;		// Scale down to 5 bits in temporary variable

		//cout << "Before: " << static_cast<unsigned short>(buffer[i]) << " -> After: " << static_cast<unsigned short>(scaledVal) << endl;

		for(int w=0 ; w<NR_BITS ; ++w) {
			revBuffer.push_back(scaledVal % 2);
			scaledVal /= 2;
		}
	}

	// Bits in revBuffer are in reverse order, so we fix that:
	for(int i=0 ; i<revBuffer.size()/NR_BITS ; ++i) {
		for(int z=0 ; z<NR_BITS ; ++z) {
			fileBuffer.push_back(revBuffer[NR_BITS-1-z + i*NR_BITS]);	// Reverse every 5 bits
		}
	}


	// Print out:
	// -------------------------------------------------------- REVERSE BUFFER:
	cout << "revBuffer:  ";
	for(int i=0 ; i<revBuffer.size() ; ++i) {
		cout << revBuffer[i];
		if((i+1) % NR_BITS == 0) cout << " ";
	}
	cout << endl;

	// -------------------------------------------------------- FILE BUFFER:
	cout << "fileBuffer: ";
	for(int i=0 ; i<fileBuffer.size() ; ++i) {
		cout << fileBuffer[i];
		if((i+1) % NR_BITS == 0) cout << " ";
	} 
	cout << endl;

	/*
	// FIXME - this commented out code is not working as expected

	// Bit manipulations code:
	// http://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit-in-c-c

	unsigned char anotherBuffer[static_cast<int>(fileBuffer.size()/8.0 + 0.5)];
	cout << sizeof(anotherBuffer)/sizeof(anotherBuffer[0]) << "  " << fileBuffer.size() << endl;

	for(int t=0 ; t<sizeof(anotherBuffer)/sizeof(anotherBuffer[0]) ; ++t) {
		for(int h=0 ; h<NR_BITS ; ++h) 
			anotherBuffer[t] ^= (-fileBuffer[t*NR_BITS + h] ^ anotherBuffer[t]) & (1 << h);
	}
	*/


	/*
	// FIXME - this commented out code is not working as expected

	//void* bufferFront = &fileBuffer[0];		// DOESN'T COMPILE, CAUSES ERROR
	void* bufferFront = &fileBuffer;		// Put pointer at the beginning of the buffer memory

	outputFile.write(static_cast<char*>(bufferFront), static_cast<int>(fileBuffer.size()/8.0 + 0.5));
	*/

	// Writing from buffer (std::vector<sf::Uint8>) to outputFile (std::ofstream):
	void* bufferFront = &buffer[0];		// Put pointer at the beginning of the buffer memory
	outputFile.write(static_cast<char*>(bufferFront), buffer.size());

	outputFile.close();
}

