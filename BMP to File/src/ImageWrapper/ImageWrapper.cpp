#include "ImageWrapper.hpp"

#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

ImageWrapper::ImageWrapper() :
	loadedFlag(false)
{}


void ImageWrapper::Load(const string& pathWname) {
	if(img.loadFromFile(pathWname)) {
		cout << pathWname <<" successfully loaded\n";
		loadedFlag = true;
	}
	else cout << "Error while loading image.\n";
}


void ImageWrapper::ExportFile(unsigned char codingType, bool grayscale) {
	if(!loadedFlag) return;

	outputFile.ExportFromImg(img, codingType, grayscale, "../data/output.file");
	cout << "Binary file exported (output.file)\n";
}

