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
		cout << " -> Bitmap " << pathWname <<" successfully loaded\n";
		loadedFlag = true;
	}
	else cout << " -> Error while loading image (" << pathWname << ")\n";
}


void ImageWrapper::ExportFile(unsigned char codingType, bool grayscale) {
	if(!loadedFlag) return;

	string outputFilename = "../data/output.file";

	if(outputFile.ExportFromImg(img, codingType, grayscale, outputFilename)) {
		cout << " -> Binary file exported (" << outputFilename << ")\n";
	}
	else cout << " -> Error while exporting binary file (" << outputFilename << ")\n";
}

