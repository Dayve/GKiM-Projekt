#ifndef _BINARYFILE_H_
#define _BINARYFILE_H_

#include "../Block/Block.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdint.h>

class BinaryFile {
	public:
		BinaryFile() : imgW(0), imgH(0) {}
		~BinaryFile() {}

		void ExportFromImg(sf::Image& image, bool codingType, bool grayscale);
		bool ImportFromFile(const std::string& pathWithName);

		sf::Uint8* getValuesAddress();
		unsigned getW();
		unsigned getH();

	private:
		uint16_t imgW, imgH;
		std::vector<sf::Uint8> values;
		std::vector<Block> blocks;
};

#endif


