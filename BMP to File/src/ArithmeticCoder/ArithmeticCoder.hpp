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
		std::map<sf::Uint8, int> Occurrences;
		std::map<sf::Uint8, double> Probabilities;

		void AssignOccurrences();
		void AssignProbabilities();

		void PrintOut();

		std::vector<sf::Uint8>* buffer;		// Remote buffer pointer
};

#endif


