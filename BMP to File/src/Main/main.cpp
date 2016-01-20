#include <iostream>
#include <cstdlib>
#include "../ImageWrapper/ImageWrapper.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
	string pathWithFilename, codingMethodsName;
	bool grayscale;
    unsigned char codingType;
	ImageWrapper ImgObj;

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" path/to/filename.bmp 0/1/2(coding type) 0/1(grayscale)\n";
	}
	else {
		pathWithFilename = argv[1];
		codingType = atoi(argv[2]);
		grayscale = atoi(argv[3]);

        switch(codingType) {
            case 0: codingMethodsName = "Arithmetic Coding"; break;
            case 1: codingMethodsName = "Byterun"; break;
            case 2: codingMethodsName = "Scaling to 5-bit values"; break;
        }

		cout << "CHOSEN OPTIONS: ----------------------\n";
		cout << " Grayscale: " << (grayscale ? "Yes" : "No") << endl;
		cout << " Coding type: " << codingMethodsName << endl;
		cout << "--------------------------------------\n";

		ImgObj.Load(pathWithFilename);
		ImgObj.ExportFile(codingType, grayscale);
	}

	return 0;
}
