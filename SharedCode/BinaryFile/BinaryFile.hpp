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

		void ExportFromImg(sf::Image& image, bool codingType, bool grayscale, const std::string& resultPathWithName);
        void ExportFromImg_coding(sf::Image& image, bool codingType, bool grayscale, const string& resultPathWithName);
		bool ImportFromFile(const std::string& pathWithName);

		sf::Uint8* getPxValuesAddress(); // Used in FileWrapper::ExportBMP(), as a parameter for sf::Image::create()
		unsigned getW();
		unsigned getH();

	private:
		uint16_t imgW, imgH;
		std::vector<sf::Uint8> pixelValues;
		std::vector<Block> blocks;

        void FetchValuesFromImg(sf::Image& image);
};

#endif


