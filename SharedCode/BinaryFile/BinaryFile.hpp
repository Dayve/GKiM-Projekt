#ifndef _BINARYFILE_H_
#define _BINARYFILE_H_

#include "../Block/Block.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdint.h>

class BinaryFile {
	public:
		BinaryFile() : blocks{ Block() } {}
		~BinaryFile() {}

		void ExportFromImg(sf::Image& image, bool codingType, bool grayscale, const std::string& dataDir);
		void Load(const std::string& fullPath);

		void PrintOutFile();	// [DEBUG ONLY]

	private:
		std::vector<Block> blocks;
		std::vector<sf::Uint8> values;
};

#endif


