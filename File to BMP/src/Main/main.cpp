#include <iostream>
#include "../FileWrapper/FileWrapper.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
	string pathWithFilename;
	FileWrapper FileObj;

	if(argc != 2) {
		cout << "Usage: "<<argv[0]<<" path/to/filename.file\n";
	}
	else {
		pathWithFilename = argv[1];

		FileObj.Load(pathWithFilename);
		FileObj.ExportBMP();
	}

	return 0;
}
