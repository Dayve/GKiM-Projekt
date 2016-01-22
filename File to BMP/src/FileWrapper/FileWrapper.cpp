#include "FileWrapper.hpp"
using namespace std;

FileWrapper::FileWrapper() :
	loadedFlag(false)
{}


void FileWrapper::Load(const string& pathWname) {
	if(inputFile.ImportFromFile(pathWname)) {
		loadedFlag = true;
	}
	else cout << " -> Error while importing from a binary file\n";
}


void FileWrapper::ExportBMP() {
	if(!loadedFlag) return;

	string resultFilename = "../data/result.bmp";

	resultImg.create(inputFile.getW(), inputFile.getH(), inputFile.getPxValuesAddress());
	resultImg.saveToFile(resultFilename);	// TODO: Change path if needed

	cout << " -> BMP exported (" << resultFilename << ")\n";
}
