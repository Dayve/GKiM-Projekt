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
	vector<bool> bitBuffer;		

	for(int i=0 ; i<buffer.size() ; ++i) {
		sf::Uint8 scaledVal = (buffer[i] * (pow(2, NR_BITS)-1))/255;		// Scale down to 5 bits in temporary variable

		cout << "Before: " << static_cast<unsigned short>(buffer[i]) << " -> After: " << static_cast<unsigned short>(scaledVal) << endl;

		for(int w=0 ; w<NR_BITS ; ++w) {
			bitBuffer.push_back(scaledVal % 2);
			scaledVal /= 2;
		}
	}

	// -------------------------------------------------------- BIT BUFFER:
	cout << "revBuffer before: ";
	for(int i=0 ; i<bitBuffer.size() ; ++i) {
		cout << bitBuffer[i];
		if((i+1) % NR_BITS == 0) cout << " ";
	}
	cout << endl;

	// Every NR_BITS bits in bitBuffer are in reverse order, so we fix that:
	for(int i=0 ; i<bitBuffer.size()/NR_BITS ; ++i) {
		// NR_BITS/2 will be rounded down for odd numbers, but this is fine, because the middle bit won't be swapped:
		for(int j=0 ; j<NR_BITS/2 ; ++j) swap(bitBuffer[j + i*NR_BITS], bitBuffer[NR_BITS-1-j + i*NR_BITS]);
	}

	// -------------------------------------------------------- BIT BUFFER:
	cout << "revBuffer after:  ";
	for(int i=0 ; i<bitBuffer.size() ; ++i) {
		cout << bitBuffer[i];
		if((i+1) % NR_BITS == 0) cout << " ";
	}
	cout << endl;
	
	/* TODO:
	 * We have all the bits in vector<bool> bitBuffer. That's good, because we will need them anyway.
	 * Even if we will use structure, containing 5 bytes (5, because it's 8*5 bits), we will have to put every single
	 * bit in it, so here's why bitBuffer is not totally useless.
	 */

	void* bufferFront = &buffer[0];		// Put pointer at the beginning of the buffer memory
	outputFile.write(static_cast<char*>(bufferFront), buffer.size());

	outputFile.close();
}

