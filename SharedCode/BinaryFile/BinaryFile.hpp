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

		void ExportFromImg(sf::Image& image, bool codingType, bool grayscale, const std::string& dataDir);
		bool ImportFromFile(const std::string& fullPath);

		void PrintOutFile();	// [DEBUG ONLY]

		std::vector<sf::Uint8> values;
		uint16_t imgW, imgH;

	private:
		std::vector<Block> blocks;
};

#endif


