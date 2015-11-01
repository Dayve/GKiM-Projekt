#include "ImageWrapper.hpp"
#include <iostream>
#include <fstream>

using namespace std;

ImageWrapper::ImageWrapper() :
	loadedFlag(false)
{}


ImageWrapper::~ImageWrapper() {}


void ImageWrapper::Load(const string& filename) {
	if(img.loadFromFile(filename))
		cout << filename <<" loaded\n";
	else
		cout << "Error while loading image.\n";
	loadedFlag = true;
}


void ImageWrapper::ExportFile(bool codingType, bool grayscale, const string& dataDir) {
	if(!loadedFlag) return;

	string fullPath = dataDir + "output.file";
	ofstream outputFile(fullPath.c_str(), ios::binary | ios::out);

//	Fetching from img (sf::Image) to buffer (std::vector<sf::Uint8>):
	for(unsigned int yy=0 ; yy<img.getSize().y ; ++yy) {
		for(unsigned int xx=0 ; xx<img.getSize().x ; ++xx) {
			buffer.push_back(img.getPixel(xx, yy).r);
			buffer.push_back(img.getPixel(xx, yy).g);
			buffer.push_back(img.getPixel(xx, yy).b);
		}
	}

//	Writing from buffer (std::vector<sf::Uint8>) to outputFile (std::ofstream):
	const char* bufferFront = reinterpret_cast<char*>(&buffer[0]);
	outputFile.write(bufferFront, buffer.size());

	outputFile.close();
}

