#include "ImageWrapper.hpp"
#include <iostream>
using namespace std;

void ImageWrapper::Load(const std::string &filename) {
	if(img.loadFromFile(filename))
		cout << filename <<" loaded\n";
	else
		cout << "Error while loading image.\n";
}

