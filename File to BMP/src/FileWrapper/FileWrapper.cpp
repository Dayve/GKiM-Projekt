#include "FileWrapper.hpp"
using namespace std;

FileWrapper::FileWrapper() :
	loadedFlag(false)
{}


void FileWrapper::Load(const string& pathWname) {
	if(inputFile.ImportFromFile(pathWname)) {
		cout << pathWname <<" successfully loaded\n";
		loadedFlag = true;
	}
	else cout << "Error while importing from a binary file.\n";
}


void FileWrapper::ExportBMP() {
	if(!loadedFlag) return;

	resultImg.create(inputFile.getW(), inputFile.getH(), inputFile.getPxValuesAddress());
	resultImg.saveToFile("../data/result.bmp");	// TODO: Change path if needed

	cout << "BMP exported (result.bmp)\n";
}
