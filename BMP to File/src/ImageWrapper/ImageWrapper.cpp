#include "ImageWrapper.hpp"
#include <iostream>
#include <fstream>

using namespace std;

ImageWrapper::ImageWrapper() :
	loadedFlag(false)
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

//	TODO: Coding and grayscale

	string fullPath = dataDir + "output.file";
	ofstream outputFile(fullPath.c_str(), ios::binary | ios::out);

//	Fetching from img (sf::Image -> sf::Uint8) to buffer (std::vector<sf::Uint8>):
	for(unsigned int yy=0 ; yy<img.getSize().y ; ++yy) {
		for(unsigned int xx=0 ; xx<img.getSize().x ; ++xx) {
			buffer.push_back(img.getPixel(xx, yy).r);
			buffer.push_back(img.getPixel(xx, yy).g);
			buffer.push_back(img.getPixel(xx, yy).b);
		}
	}

//	Writing from buffer (std::vector<sf::Uint8>) to outputFile (std::ofstream):
	void* bufferFront = &buffer[0];
	outputFile.write(static_cast<char*>(bufferFront), buffer.size());

	outputFile.close();
}

