#ifndef _BINARYFILE_H_
#define _BINARYFILE_H_

#include "../Block/Block.hpp"
#include "../ByteRun/ByteRun.hpp"
#include "../ArithmeticCoding/ArithmeticCoding.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <string>

class BinaryFile {
    public:
        BinaryFile() : imgW(0), imgH(0) {}
        ~BinaryFile() {}

        void ExportFromImg(sf::Image& image, unsigned char codingType, bool grayscale, const std::string& resultPathWithName);
        bool ImportFromFile(const std::string& pathWithName);

        sf::Uint8* getPxValuesAddress(); // Used in FileWrapper::ExportBMP(), as a parameter for sf::Image::create()
        unsigned getW();
        unsigned getH();

    private:
        uint16_t imgW, imgH;
        std::vector<sf::Uint8> pixelValues;
        std::vector<Block> blocks;

        void FetchValuesFromImg(sf::Image& image);

        // Compression:
        ArithmeticCoding ACoding;
        ByteRun BRun;
};

#endif


