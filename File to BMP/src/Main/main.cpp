#include <iostream>
#include "../FileWrapper/FileWrapper.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
/*	
 *	dataPath could be configured to be set in CMakeLists.txt, possibly files could be shared 
 *	between subprojects for testing purposes (requires some changes in project tree)
 */
	string filename, dataPath = "../data/"; 
	//FileWrapper FileObj;

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" filename\n";
	}
	else {
		filename = argv[1]; 		// TODO: Check if some function like createFromCstr() should be used here

		//FileObj.Load(dataPath + filename);
		//FileObj.ExportBMP(dataPath);
	}

	return 0;
}
