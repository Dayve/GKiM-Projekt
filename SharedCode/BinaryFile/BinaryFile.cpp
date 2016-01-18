#include "BinaryFile.hpp"
#include <cmath>
#include <fstream>
#include <bitset>
using namespace std;


void BinaryFile::FetchValuesFromImg(sf::Image& image) {
	// Fetching from an image (sf::Image -> sf::Uint8) to pixelValues (std::vector<sf::Uint8>):
	for(unsigned int yy=0 ; yy<image.getSize().y ; ++yy) {
		for(unsigned int xx=0 ; xx<image.getSize().x ; ++xx) {
			pixelValues.push_back(image.getPixel(xx, yy).r);
			pixelValues.push_back(image.getPixel(xx, yy).g);
			pixelValues.push_back(image.getPixel(xx, yy).b);
		}
	}
}


void BinaryFile::ExportFromImg_coding(sf::Image& image, bool codingType, bool grayscale, const string& resultPathWithName) {
	FetchValuesFromImg(image);

    // Scale values and save them in ScaledValues vectors, depending on codingType:
	if(grayscale) {
		for(int i=0 ; i<pixelValues.size() ; i += 3) {
			sf::Uint8 avgColor = (pixelValues[i] + pixelValues[i+1] + pixelValues[i+2])/3;  // Calculate the grayscale equivalent of a given color
			sf::Uint8 scaledVal = (avgColor * (pow(2.0, Block::NR_BITS)-1))/255.0;          // Scale down to 5 bits in the temporary variable

            // Byterun: codingType=1, Arithmetic Coding: codingType=0
            if(codingType) BRun.ScaledValues.push_back(scaledVal);
            else ACoding.ScaledValues.push_back(scaledVal);
		}
	}
	else {
		for(int i=0 ; i<pixelValues.size() ; ++i) {
			sf::Uint8 scaledVal = (pixelValues[i] * (pow(2.0, Block::NR_BITS)-1))/255.0;  // Scale down to 5 bits in the temporary variable

            if(codingType) BRun.ScaledValues.push_back(scaledVal);
            else ACoding.ScaledValues.push_back(scaledVal);
		}
	}

    if(codingType) BRun.Compress();
    else ACoding.Compress();

    // TODO: 
    // Here we should save the results (from proper Results std::vector, which is now, 
    // after calling Compress() function filled with data) to binary file.
}


void BinaryFile::ExportFromImg(sf::Image& image, bool codingType, bool grayscale, const string& resultPathWithName) {
	FetchValuesFromImg(image);

	vector<bool> bitBuffer;  // Holds the results of converting the pixel values (scaled down to fit in 5 bits) from decimal to binary

	// We scale the pixel values down to 5 bits and put results in the bitBuffer:
	// (We've put for loops inside if blocks and not the other way around, because this way we check condidion only once)
	if(grayscale) {
		for(int i=0 ; i<pixelValues.size() ; i += 3) {
			sf::Uint8 avgColor = (pixelValues[i] + pixelValues[i+1] + pixelValues[i+2])/3;  // Calculate the grayscale equivalent of a given color
			sf::Uint8 scaledVal = (avgColor * (pow(2.0, Block::NR_BITS)-1))/255.0;          // Scale down to 5 bits in the temporary variable

			for(int w=0 ; w<Block::NR_BITS ; ++w) {
				bitBuffer.push_back(scaledVal % 2);
				scaledVal /= 2;
			}
		}
	}
	else {
		for(int i=0 ; i<pixelValues.size() ; ++i) {
			sf::Uint8 scaledVal = (pixelValues[i] * (pow(2.0, Block::NR_BITS)-1))/255.0;  // Scale down to 5 bits in the temporary variable

			for(int w=0 ; w<Block::NR_BITS ; ++w) {
				bitBuffer.push_back(scaledVal % 2);
				scaledVal /= 2;
			}
		}
	}

	// Every NR_BITS bits in bitBuffer are in reverse order, so we fix that:
	for(int i=0 ; i<bitBuffer.size()/Block::NR_BITS ; ++i) {
		// NR_BITS/2 will be rounded down for odd numbers, but this is fine, because the middle bit won't be swapped anyway:
		for(int j=0 ; j<Block::NR_BITS/2 ; ++j) swap(bitBuffer[j + i*Block::NR_BITS], bitBuffer[Block::NR_BITS-1-j + i*Block::NR_BITS]);
	}


	blocks.push_back(Block());  // There were no Blocks before in blocks vector, so we add the first one

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
	/* -------------------------------------------
		0-1 FOR CODING TYPE: (on MBS of imgW)
		   0 Arithmetic Coding
		   1 Byterun
		0-1 FOR GRAYSCALE: (on MSB of imgH) 
		   0 Keep colour values
		   1 Change to grayscale

		We can later deduce the number of blocks from width and height.
	------------------------------------------- */

	ofstream outputFile(resultPathWithName.c_str(), ios::binary | ios::out);

	uint16_t imgW = image.getSize().x;
	uint16_t imgH = image.getSize().y;

	if(codingType) imgW += pow(2, 15);  // Could be imgW |= 0b1000000000000000
	if(grayscale) imgH += pow(2, 15);   // Could be imgH |= 0b1000000000000000

	// Writing header:
	outputFile.write(reinterpret_cast<const char*>(&imgW), sizeof(imgW));
	outputFile.write(reinterpret_cast<const char*>(&imgH), sizeof(imgH));

	// Writing blocks (5 bytes each, hence the second parameter is "blocks.size()*Block::NR_BITS"):
	void* bufferFront = blocks[0].getBytesAddr();
	outputFile.write(static_cast<const char*>(bufferFront), blocks.size()*Block::NR_BITS);

	outputFile.close();
}



bool BinaryFile::ImportFromFile(const std::string& pathWithName) {
	ifstream inputFile(pathWithName.c_str(), ios::binary | ios::in);
	if(inputFile.fail()) return false;

	bool codingType = false, grayscale = false;

	// Read the header:
	inputFile.read(reinterpret_cast<char*>(&imgW), sizeof(imgW));
	inputFile.read(reinterpret_cast<char*>(&imgH), sizeof(imgH));

	// Check MSBs: ("most significant bits")
	if(imgW > pow(2, 15)) {
		codingType = true;
		imgW -= pow(2, 15);
	}
	if(imgH > pow(2, 15)) {
		grayscale = true;
		imgH -= pow(2, 15);
	}

	// Calculate the number of values in file, then the number of blocks:
	int numBlocks, numValuesInFile;

	if(grayscale) numValuesInFile = imgW*imgH; // One per pixel
	else numValuesInFile = imgW*imgH * 3;      // Three per pixel

    // Calculate the number of blocks based of the amount of the values in a file:
	numBlocks = ((numValuesInFile * Block::NR_BITS) % (Block::NR_BITS*8)) ? 
		((numValuesInFile * Block::NR_BITS) / (Block::NR_BITS*8)) + 1 : 
		((numValuesInFile * Block::NR_BITS) / (Block::NR_BITS*8));

	// Read bits blockwise:
	for(int w=0 ; w<numBlocks ; ++w) {
		blocks.push_back(Block());
		inputFile.read(reinterpret_cast<char*>(&blocks[w]), Block::NR_BITS); // We read one block (NR_BITS bytes) at the time 
	}

	// Print out the readed information:
	cout << "DETECTED OPTIONS: -------------\n";
	cout << " Grayscale: " << (grayscale ? "Yes" : "No") << endl;
	cout << " Coding type: " << (codingType ? "Byterun" : "Arithmetic Coding") << endl;
	cout << "-----------------------------\n";

	// For counting how many values we have already read (so that we don't read the empty bits at the end) 
	// and added (so that we know where to put the alpha channel values):
	int readedValuesCounter = 0, addedValuesCounter = 0;

	for(int i=0 ; i<blocks.size() ; ++i) {  // For every block
		for(int j=0 ; j<8 ; ++j) {          // For every five bits (there are 8 fives in every block)
			if(readedValuesCounter == numValuesInFile) return true;

			sf::Uint8 valueFromBits = 0;

			for(int k=0 ; k<Block::NR_BITS ; ++k) { // For every bit
				if(blocks[i].getBit(j*Block::NR_BITS + k)) valueFromBits += pow(2, Block::NR_BITS-1 - k);
			}

            // Scale value back to 8 bits:
			valueFromBits = (valueFromBits*255.0)/(pow(2.0, Block::NR_BITS)-1);
			readedValuesCounter++;

			int t = 1;
			if(grayscale) t = 3; // If grayscale is set we put there 3 identical values (one for each channel)

			for(int l=0 ; l<t ; ++l) {
				pixelValues.push_back(valueFromBits);
				addedValuesCounter++;
			}

			if((addedValuesCounter)%3 == 0) {
				pixelValues.push_back(255); // Alpha component (sf::Image::saveToFile needs that)
			}
		}
	}

	return true;
}


sf::Uint8* BinaryFile::getPxValuesAddress() {
	return &pixelValues[0];
}


unsigned BinaryFile::getW() {
	return imgW;	// uint16_t => unsigned int
}


unsigned BinaryFile::getH() {
	return imgH;	// uint16_t => unsigned int
}


