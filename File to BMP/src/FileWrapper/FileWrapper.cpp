#include "FileWrapper.hpp"
using namespace std;

FileWrapper::FileWrapper() :
	loadedFlag(false)
{}


void FileWrapper::Load(const string& fullPath) {
	if(inputFile.ImportFromFile(fullPath)) {
		cout << fullPath <<" successfully loaded\n";
		loadedFlag = true;
	}
	else cout << "Error while loading binary file.\n";
}


void FileWrapper::ExportBMP(const string& dataDir) {
	if(!loadedFlag) return;

	// TODO:
	// - [ ] use sf::Image::create and sf::Image::saveToFile to export BMP
}
