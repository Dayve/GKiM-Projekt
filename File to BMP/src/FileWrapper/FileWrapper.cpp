#include "FileWrapper.hpp"
using namespace std;

FileWrapper::FileWrapper() :
	loadedFlag(false)
{}


void FileWrapper::Load(const string& fullPath) {
	inputFile.Load(fullPath);
}


void FileWrapper::ExportBMP(const string& dataDir) {
	// TODO
}
