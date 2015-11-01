#include <iostream>
#include "../ImageWrapper/ImageWrapper.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
	string filename;
	bool codingType, grayscale;

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" filename 0/1(coding type) 0/1(grayscale)\n";
	}
	else {
		filename = argv[1];
		codingType = argv[2];
		grayscale = argv[3];

		ImageWrapper ImgObj;
		ImgObj.Load("../data/" + filename);
	}

	return 0;
}
