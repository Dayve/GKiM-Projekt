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

	vector<bool> bitBuffer;		// Holds results of converting values (scaled down to fit in 5 bits) from decimal to binary

	for(int i=0 ; i<buffer.size() ; ++i) {
		sf::Uint8 scaledVal = (buffer[i] * (pow(2, NR_BITS)-1))/255;		// Scale down to 5 bits in temporary variable

		cout << "Before: " << static_cast<unsigned short>(buffer[i]) << " -> After: " << static_cast<unsigned short>(scaledVal) << endl;

		for(int w=0 ; w<NR_BITS ; ++w) {
			bitBuffer.push_back(scaledVal % 2);
			scaledVal /= 2;
		}
	}


	// ########## Printing out:
	// -------------------------------------------------------- BIT BUFFER:
	cout << "bitBuffer before: ";
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
	cout << "bitBuffer after:  ";
	for(int i=0 ; i<bitBuffer.size() ; ++i) {
		cout << bitBuffer[i];
		if((i+1) % NR_BITS == 0) cout << " ";
	}
	cout << endl;
	// ##########


	vector<block> blocks = { block() };

	for(int i=0, j=0, blockIndex=0 ; i<bitBuffer.size() ; ++i, ++j) {
		if(j>39) {
			j-=40;
			blocks.push_back(block());
			blockIndex++;
		}
		if(bitBuffer[i]) blocks[blockIndex].setBit(j);
	}

	// Writing binary file header (number of blocks inside, on 4 bytes):
	uint32_t header = blocks.size();
	outputFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

	// Writing blocks (5 bytes each, hence the second parameter is "blocks.size()*NR_BITS"):
	void* bufferFront = blocks[0].getBytesAddr();
	outputFile.write(static_cast<const char*>(bufferFront), blocks.size()*NR_BITS);

	outputFile.close();

//	=================================================== Print out generated binary file:
//	(code from gist bfr, of course can be removed)

	cout << endl << "Output from 'output.file' in form: [header | body]\n";

	ifstream file ("../data/output.file", ios::in | ios::binary | ios::ate);
	streampos fileSize;
	char* bytes;

	if (file.is_open()) {
		// --------------------------------- Loading from "file" to "bytes":
		fileSize = file.tellg();
		bytes = new char [fileSize];
		file.seekg (0, ios::beg); 
		file.read(bytes, fileSize);
		file.close();

		// --------------------------------- Printing out in bytes:
		cout << "Size: " << fileSize <<" bytes:" << endl;

		for(int i=0 ; i<fileSize ; ++i) {
			bitset<8> set(bytes[i]);
			cout << set << " ";

			if(i==3) cout << " |  ";
		}
		cout << endl;	

		// --------------------------------- Printing out in fives:
		cout << "In groups of 5 bits:" << endl;

		// We put bits in vector<bool>:
		vector<bool> bits;
		for(int i=0 ; i<fileSize ; ++i) {
			bitset<8> set(bytes[i]);
			for(int j=0 ; j<8 ; ++j) {
				bits.push_back(set[7-j]);
			}
		}

		// Print out header:
		for(int i=0 ; i<4 /*header size*/; ++i) {
			bitset<8> set(bytes[i]);
			cout << set << " ";

			if(i==3) cout << " |  ";
		}

		// And the body of a file:
		for(int i=32 ; i<bits.size() ; ++i) {
			cout << bits[i];
			if((i+32+2) % 5 /*NR_BITS*/ == 0) cout << " ";
		}
		cout << endl;	

		delete[] bytes;
	}
	else cout << "Unable to open file" << endl;

//	===================================================
}

