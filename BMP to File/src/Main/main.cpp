#include <iostream>
#include <cstdlib>
#include "../ImageWrapper/ImageWrapper.hpp"

#include "../../../SharedCode/ArithmeticCoding/ArithmeticCoding.hpp" // REMOVE

using namespace std; 

int main(int argc, char* argv[]) {
	string pathWithFilename;
	bool codingType, grayscale;
	ImageWrapper ImgObj;

    ArithmeticCoding AC; // REMOVE
    AC.Test(); // REMOVE

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" path/to/filename.bmp 0/1(coding type) 0/1(grayscale)\n";
	}
	else {
		pathWithFilename = argv[1];
		codingType = atoi(argv[2]);
		grayscale = atoi(argv[3]);

		cout << "CHOSEN OPTIONS: -------------\n";
		cout << " Grayscale: " << (grayscale ? "Yes" : "No") << endl;
		cout << " Coding type: " << (codingType ? "Byterun" : "Arithmetic Coding") << endl;
		cout << "-----------------------------\n";

		ImgObj.Load(pathWithFilename);
		ImgObj.ExportFile(codingType, grayscale);
	}

	return 0;
}
