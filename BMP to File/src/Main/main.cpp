#include <SFML/Graphics.hpp> // Not needed here
#include <iostream>

#include "../Class1/class1.hpp"

using namespace std; 

int main(int argc, char* argv[]) {
	string filename;
	bool codingType, grayscale;

	sf::Image img;

	if(argc != 4) {
		cout << "Usage: "<<argv[0]<<" filename 0/1(coding type) 0/1(grayscale)\n";
	}
	else {
		filename = argv[1];
		codingType = argv[2];
		grayscale = argv[3];
	}

	Class1 obj;
	obj.fun();

	return 0;
}
