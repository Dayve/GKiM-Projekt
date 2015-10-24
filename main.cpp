#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std; 

int main() { 
	sf::Image img;
	if(!img.loadFromFile("sq.bmp")) {
		cout << "Not loaded\n";
		return -1;
	}
	else cout << "Loaded\n";
	
	unsigned int x = img.getSize().x;
	unsigned int y = img.getSize().y;

	cout << "x: " << x << ", y: " << y << endl;

	for(unsigned int i=0 ; i<y ; ++i) {
		for(unsigned int j=0 ; j<x ; ++j) {
			//sf::Uint8 r, g, b, a;
			unsigned short r, g, b, a;

			r = img.getPixel(j, i).r;
			g = img.getPixel(j, i).g;
			b = img.getPixel(j, i).b;

			cout<<"[r="<<r<<" g="<<g<<" b="<<b<<"]  ";
		}
		cout<<endl;
	}

	return 0;
}
