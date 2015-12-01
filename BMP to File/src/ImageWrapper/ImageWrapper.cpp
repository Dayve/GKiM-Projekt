#include "ImageWrapper.hpp"

#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

ImageWrapper::ImageWrapper() :
	loadedFlag(false)
{}

void ImageWrapper::Load(const string& fullPath) {
	if(img.loadFromFile(fullPath)) {
		cout << fullPath <<" loaded\n";
		loadedFlag = true;
	}
	else cout << "Error while loading image.\n";
}


void ImageWrapper::ExportFile(bool codingType, bool grayscale, const string& dataDir) {
	if(!loadedFlag) return;
	else outputFile.ExportFromImg(img, codingType, grayscale, dataDir);
}

