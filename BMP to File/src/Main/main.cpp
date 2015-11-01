#include <iostream>
#include "../ImageWrapper/ImageWrapper.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
/*	
 *	dataPath could be configured to be set in CMakeLists.txt, possibly files could be shared 
 *	between subprojects for testing purposes (requires some changes in project tree)
 */
	string filename, dataPath = "../data/"; 
	bool codingType, grayscale;
	ImageWrapper ImgObj;

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" filename 0/1(coding type) 0/1(grayscale)\n";
	}
	else {
		filename = argv[1];
		codingType = argv[2];
		grayscale = argv[3];

		ImgObj.Load(dataPath + filename);
		ImgObj.ExportFile(codingType, grayscale, dataPath);
	}

	return 0;
}
