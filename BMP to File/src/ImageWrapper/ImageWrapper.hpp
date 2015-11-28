#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include <cmath>	// For pow() in block::set

#include "../ArithmeticCoder/ArithmeticCoder.hpp"

class ImageWrapper {
	private:
		sf::Image img;
		bool loadedFlag;
		std::vector<sf::Uint8> buffer;

		ArithmeticCoder AC;

	public:
		static const int NR_BITS = 5;

		ImageWrapper();
		~ImageWrapper();

		void Load(const std::string& filename);
		void ExportFile(bool codingType, bool grayscale, const std::string& dataDir);
};

struct block {
	uint8_t bytes[ImageWrapper::NR_BITS];

	block() : bytes{0} {}

	void setBit(int whichBit) {
		if(whichBit > 39) return;			// whichBit can range from 0 to 39

		int posInByte = whichBit % 8;
		int whichByte = whichBit / 8;

		uint8_t temp = pow(2, 7-posInByte);	// Set 1 at (7-posInByte)th position in temp byte

		bytes[whichByte] |= temp;			// "or" together temp byte and corresponding byte from array
	}

	void* getBytesAddr() {
		return &bytes[0];
	}
};

#endif
