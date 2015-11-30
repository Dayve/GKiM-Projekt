#include "BinaryFile.hpp"
#include <cmath>
#include <fstream>
#include <bitset>
using namespace std;

void BinaryFile::ExportFromImg(sf::Image& image, bool codingType, bool grayscale, const string& dataDir) {
	// Fetching from image (sf::Image -> sf::Uint8) to values (std::vector<sf::Uint8>):
	for(unsigned int yy=0 ; yy<image.getSize().y ; ++yy) {
		for(unsigned int xx=0 ; xx<image.getSize().x ; ++xx) {
			values.push_back(image.getPixel(xx, yy).r);
			values.push_back(image.getPixel(xx, yy).g);
			values.push_back(image.getPixel(xx, yy).b);
		}
	}

	vector<bool> bitBuffer;		// Holds results of converting values (scaled down to fit in 5 bits) from decimal to binary

	// We scale values down to 5 bits and put results in bitBuffer:
	// (We've put for loops inside if blocks and not the other way around, because this way we check condidion only once)
	if(grayscale) {
		for(int i=0 ; i<values.size() ; i += 3) {
			sf::Uint8 avgColor = (values[i] + values[i+1] + values[i+2])/3;	// Calculate the grayscale equivalent of given color
			sf::Uint8 scaledVal = (avgColor * (pow(2, Block::NR_BITS)-1))/255;		// Scale down to 5 bits in temporary variable

			for(int w=0 ; w<Block::NR_BITS ; ++w) {
				bitBuffer.push_back(scaledVal % 2);
				scaledVal /= 2;
			}
		}
	}
	else {
		for(int i=0 ; i<values.size() ; ++i) {
			sf::Uint8 scaledVal = (values[i] * (pow(2, Block::NR_BITS)-1))/255;		// Scale down to 5 bits in temporary variable

			for(int w=0 ; w<Block::NR_BITS ; ++w) {
				bitBuffer.push_back(scaledVal % 2);
				scaledVal /= 2;
			}
		}
	}


	// ########## Printing out: [DEBUG ONLY]
	// -------------------------------------------------------- BIT BUFFER BEFORE:
	cout << "bitBuffer before: ";
	for(int i=0 ; i<bitBuffer.size() ; ++i) {
		cout << bitBuffer[i];
		if((i+1) % Block::NR_BITS == 0) cout << " ";
	}
	cout << endl;
	// ##########


	// Every NR_BITS bits in bitBuffer are in reverse order, so we fix that:
	for(int i=0 ; i<bitBuffer.size()/Block::NR_BITS ; ++i) {
		// NR_BITS/2 will be rounded down for odd numbers, but this is fine, because the middle bit won't be swapped:
		for(int j=0 ; j<Block::NR_BITS/2 ; ++j) swap(bitBuffer[j + i*Block::NR_BITS], bitBuffer[Block::NR_BITS-1-j + i*Block::NR_BITS]);
	}


	// ########## Printing out: [DEBUG ONLY]
	// -------------------------------------------------------- BIT BUFFER AFTER:
	cout << "bitBuffer after:  ";
	for(int i=0 ; i<bitBuffer.size() ; ++i) {
		cout << bitBuffer[i];
		if((i+1) % Block::NR_BITS == 0) cout << " ";
	}
	cout << endl;
	// ##########
	

	// Here we put bits from bitBuffer into blocks of 40 bits (5*8 bytes):
	for(int i=0, j=0, blockIndex=0 ; i<bitBuffer.size() ; ++i, ++j) {
		if(j > (Block::NR_BITS*8)-1) {
			j -= Block::NR_BITS*8;
			blocks.push_back(Block());
			blockIndex++;
		}
		if(bitBuffer[i]) blocks[blockIndex].setBit(j);
	}

	// Binary file header (coding type, image width (px), grayscale, image height (px) on 4 bytes):
	/*	-------------------------------------------
		0-1 FOR CODING TYPE: (on MBS of imgW)
		   0 Arithmetic Coding
		   1 Byterun
		0-1 FOR GRAYSCALE: (on MSB of imgH) 
		   0 Keep colour values
		   1 Change to grayscale

		We can later deduce the number of blocks from width and height.
		------------------------------------------- */

	string fullPath = dataDir + "output.file";			// Concatenate strings to obtain path to file
	ofstream outputFile(fullPath.c_str(), ios::binary | ios::out);

	uint16_t imgW = image.getSize().x;
	uint16_t imgH = image.getSize().y;

	if(codingType) imgW += pow(2, 15);		// Could be imgW |= 0b1000000000000000
	if(grayscale) imgH += pow(2, 15);		// Could be imgH |= 0b1000000000000000

	// Writin header:
	outputFile.write(reinterpret_cast<const char*>(&imgW), sizeof(imgW));
	outputFile.write(reinterpret_cast<const char*>(&imgH), sizeof(imgH));

	// Writing blocks (5 bytes each, hence the second parameter is "blocks.size()*Block::NR_BITS"):
	void* bufferFront = blocks[0].getBytesAddr();
	outputFile.write(static_cast<const char*>(bufferFront), blocks.size()*Block::NR_BITS);

	outputFile.close();

	// Check result: [DEBUG ONLY]
	PrintOutFile();
}



void BinaryFile::Load(const std::string& fullPath) {
	ifstream inputFile(fullPath.c_str(), ios::binary | ios::in);

	uint16_t imgW, imgH;
	bool codingType, grayscale;

	// Read header:
	inputFile.read(reinterpret_cast<char*>(&imgW), sizeof(imgW));
	inputFile.read(reinterpret_cast<char*>(&imgH), sizeof(imgH));

	// Check MSBs:
	if(imgW > pow(2, 15)) {
		codingType = true;
		imgW -= pow(2, 15);
	}
	if(imgH > pow(2, 15)) {
		grayscale = true;
		imgH -= pow(2, 15);
	}

	// Calculate number of blocks:
	// (If values don't fit exactly in some amount of blocks add one to the amount they take, otherwise, get this amount)
	int numBlocks = ((imgW*imgH*3 * 5) % 40) ? ((imgW*imgH*3 * 5) / 40)+1 : ((imgW*imgH*3 * 5) / 40);

	// TODO:
	// - read values to blocks (std::vector)
	// - write code of Block::getBit
	// - use Block::getBit to translate binary values and put them into values (std::vector)
	// - use sf::Image::create and sf::Image::saveToFile to export BMP
}



void BinaryFile::PrintOutFile() {
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
			if((i+32+2) % 5 /*Block::NR_BITS*/ == 0) cout << " ";
		}
		cout << endl;	

		delete[] bytes;
	}
	else cout << "Unable to open file" << endl;
}


