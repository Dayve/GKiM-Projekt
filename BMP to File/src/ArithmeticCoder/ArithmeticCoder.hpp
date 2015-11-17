#ifndef _ARITHMETICCODER_H_
#define _ARITHMETICCODER_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

class ArithmeticCoder {
	public:
		ArithmeticCoder();
		ArithmeticCoder(std::vector<sf::Uint8>* ptr);
		~ArithmeticCoder();

		void Encode();

	private:
		std::map<sf::Uint8, int> Occurences;
		std::map<sf::Uint8, double> Probabilities;

		void AssignOccurences();
		void AssignProbabilities();

		void PrintOut();

		std::vector<sf::Uint8>* buffer;		// Remote buffer pointer
};

#endif


