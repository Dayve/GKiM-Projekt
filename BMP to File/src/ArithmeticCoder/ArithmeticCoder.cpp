#include "ArithmeticCoder.hpp"
#include <iostream>

using namespace std;

ArithmeticCoder::ArithmeticCoder() {
	buffer = nullptr;
}


ArithmeticCoder::ArithmeticCoder(std::vector<sf::Uint8>* ptr) {
	buffer = ptr;
}


ArithmeticCoder::~ArithmeticCoder() {
}


void ArithmeticCoder::AssignOccurrences() {
	map<sf::Uint8, int>::iterator ocIter;		// For holding a search result only

	// For every pixel's component value in buffer:
	for(int i=0 ; i<(*buffer).size() ; ++i) {
		// If a pixel's component value is has not yet occured (as we iterate through buffer) add it to map.
		// Else, increase the number of it's occurences.
		if((ocIter = Occurences.find((*buffer)[i])) == Occurences.end()) Occurences[(*buffer)[i]] = 1;
		else Occurences[(*buffer)[i]]++;
	}
}


void ArithmeticCoder::AssignProbabilities() {
	// For every pixel's component value in buffer:
	for(int i=0 ; i<(*buffer).size() ; ++i) {
		Probabilities[(*buffer)[i]] = static_cast<double>(Occurences[(*buffer)[i]])/(*buffer).size();
	}
}


void ArithmeticCoder::PrintOut() {
//	Printing out - not the best idea for large amounts of pixels:
	cout << " Occurrences:\n";
	for(auto elem : Occurrences) {	// Type of the variable will be automatically deduced from its initializer (C++11)
		int prin = static_cast<int>(elem.first);	// We use casting to int only because cout can't handle sf::Uint8
		cout << prin << " " << elem.second << "\n";
	}

	cout << " Probabilities:\n";
	for(auto elem : Probabilities) {
		int prin = static_cast<int>(elem.first);
		cout << prin << " " << elem.second << "\n";
	}

	cout << " Buffer (in order as in file):\n";		// Every map is always sorted by key (in ascending order)
	for(auto elem : *buffer) {
		cout << static_cast<unsigned>(elem) << "\n";
	}
}


void ArithmeticCoder::Encode() {
	AssignOccurrences();
	AssignProbabilities();

	PrintOut();

//	Getting the fractional value:
	double upperBound = 0.0, lowerBound = 0.0;

	/* TODO
	for(int i=0 ; i<(*buffer).size() ; ++i) {
	}
	*/
}

