#include "BinaryFile.hpp"
#include <cmath>
#include <fstream>
#include <bitset>
using namespace std;


void BinaryFile::FetchValuesFromImg(sf::Image& image) {
    // Fetching from an image (sf::Image -> sf::Uint8) to pixelValues (std::vector<sf::Uint8>):
    for(unsigned int yy=0 ; yy<image.getSize().y ; ++yy) {
        for(unsigned int xx=0 ; xx<image.getSize().x ; ++xx) {
            pixelValues.push_back(image.getPixel(xx, yy).r);
            pixelValues.push_back(image.getPixel(xx, yy).g);
            pixelValues.push_back(image.getPixel(xx, yy).b);
        }
    }
}


void BinaryFile::ExportFromImg(sf::Image& image, unsigned char codingType, bool grayscale, const string& resultPathWithName) {
    FetchValuesFromImg(image);

    vector<bool> bitBuffer;  // Holds the results of converting the pixel values (scaled down to fit in 5 bits) from decimal to binary

    // We scale the pixel values down to 5 bits and put results in the bitBuffer:
    // (We've put for loops inside if blocks and not the other way around, because this way we check condidion only once)
    if(grayscale) {
        for(int i=0 ; i<pixelValues.size() ; i += 3) {
            sf::Uint8 avgColor = (pixelValues[i] + pixelValues[i+1] + pixelValues[i+2])/3;  // Calculate the grayscale equivalent of a given color
            sf::Uint8 scaledVal = (avgColor * (pow(2.0, Block::NR_BITS)-1))/255.0;          // Scale down to 5 bits in the temporary variable
            
            if(codingType == 2) {   // Plain scaling to 5-bit values
                for(int w=0 ; w<Block::NR_BITS ; ++w) {
                    bitBuffer.push_back(scaledVal % 2);
                    scaledVal /= 2;
                }
            }
            else {  // Using compression algorithms: (Byterun - codingType=1, Arithmetic Coding - codingType=0)
                if(codingType) BRun.ScaledValues.push_back(scaledVal);
                else ACoding.ScaledValues.push_back(scaledVal);
            }
        }
    }
    else {
        for(int i=0 ; i<pixelValues.size() ; ++i) {
            sf::Uint8 scaledVal = (pixelValues[i] * (pow(2.0, Block::NR_BITS)-1))/255.0;  // Scale down to 5 bits in the temporary variable

            if(codingType == 2) {
                for(int w=0 ; w<Block::NR_BITS ; ++w) {
                    bitBuffer.push_back(scaledVal % 2);
                    scaledVal /= 2;
                }
            }
            else {
                if(codingType) BRun.ScaledValues.push_back(scaledVal);
                else ACoding.ScaledValues.push_back(scaledVal);
            }
        }
    }

    if(codingType == 2) {
        // Every NR_BITS bits in bitBuffer are in reverse order, so we fix that:
        for(int i=0 ; i<bitBuffer.size()/Block::NR_BITS ; ++i) {
            // NR_BITS/2 will be rounded down for odd numbers, but this is fine, because the middle bit won't be swapped anyway:
            for(int j=0 ; j<Block::NR_BITS/2 ; ++j) swap(bitBuffer[j + i*Block::NR_BITS], bitBuffer[Block::NR_BITS-1-j + i*Block::NR_BITS]);
        }

        blocks.push_back(Block());  // There were no Blocks before in blocks vector, so we add the first one

        // Here we put bits from bitBuffer into blocks of 40 bits (5*8 bytes):
        for(int i=0, j=0, blockIndex=0 ; i<bitBuffer.size() ; ++i, ++j) {
            if(j > (Block::NR_BITS*8)-1) {
                j -= Block::NR_BITS*8;
                blocks.push_back(Block());
                blockIndex++;
            }
            if(bitBuffer[i]) blocks[blockIndex].setBit(j);
        }
    }
    else {
        if(codingType) BRun.Compress();
        else ACoding.Compress();
    }

    /* ---------------------------------------------------------------------------------------
    Header of the binary file:
     1) 2 Bytes for width of an image
     2) 2 Bytes for height of an image
     3) 1 Byte for coding type and grayscale info:
      a) Grayscale - on most significant bit (0 colored, 1 grayscale)
      b) Coding type - as a number (0 Arithmetic Coding, 1 Byterun, 2 scaling to 5-bit values)

     We can later deduce the number of blocks/pixels from width and height.
     -------------------------------------------------------------------------------------- */

    ofstream outputFile(resultPathWithName.c_str(), ios::binary | ios::out);

    imgW = image.getSize().x;
    imgH = image.getSize().y;
    unsigned char codingAndGrayscaleData = codingType;

    if(grayscale) codingAndGrayscaleData |= 0b10000000;     // C++14
    //if(grayscale) codingAndGrayscaleData += pow(2, 7);

    // Writing header:
    outputFile.write(reinterpret_cast<const char*>(&imgW), sizeof(imgW));
    outputFile.write(reinterpret_cast<const char*>(&imgH), sizeof(imgH));
    outputFile.write(reinterpret_cast<const char*>(&codingAndGrayscaleData), sizeof(codingAndGrayscaleData));

    if(codingType == 2) {
        // Writing blocks (5 bytes each, hence the second parameter is "blocks.size()*Block::NR_BITS"):
        void* bufferFront = blocks[0].getBytesAddr();
        outputFile.write(static_cast<const char*>(bufferFront), blocks.size()*Block::NR_BITS);
    }
    else {
        // Writing results:
        if(codingType) outputFile.write(static_cast<const char*>(BRun.GetResultsAddr()), BRun.Results.size());
        else outputFile.write(static_cast<const char*>(ACoding.GetResultsAddr()), ACoding.Results.size() * sizeof(float));
    }

    outputFile.close();
}



bool BinaryFile::ImportFromFile(const std::string& pathWithName) {
    ifstream inputFile(pathWithName.c_str(), ios::binary | ios::in);
    if(inputFile.fail()) return false;

    bool grayscale = false;
    unsigned char codingType;

    // Read the header:
    inputFile.read(reinterpret_cast<char*>(&imgW), sizeof(imgW));
    inputFile.read(reinterpret_cast<char*>(&imgH), sizeof(imgH));

    inputFile.read(reinterpret_cast<char*>(&codingType), sizeof(codingType));

    if(codingType > pow(2,7)) { // Grayscale option on MSB of that byte
        grayscale = true;
        codingType -= pow(2, 7);
    }

    // Print out the readed information:
    string codingMethodsName;

    switch(codingType) {
        case 0: codingMethodsName = "Arithmetic Coding"; break;
        case 1: codingMethodsName = "Byterun"; break;
        case 2: codingMethodsName = "Scaling to 5-bit values"; break;
    }

    cout << "DETECTED OPTIONS: --------------------\n";
    cout << " Grayscale: " << (grayscale ? "Yes" : "No") << endl;
    cout << " Coding type: " << codingMethodsName << endl;
    cout << "--------------------------------------\n";


    // Calculate the number of values in file:
    int numValuesInFile;

    if(grayscale) numValuesInFile = imgW*imgH; // One per pixel
    else numValuesInFile = imgW*imgH * 3;      // Three per pixel


    if (codingType == 2) {
        // Calculate the number of blocks based of the amount of the values in a file:
        int numBlocks = ((numValuesInFile * Block::NR_BITS) % (Block::NR_BITS*8)) ? 
                        ((numValuesInFile * Block::NR_BITS) / (Block::NR_BITS*8)) + 1 : 
                        ((numValuesInFile * Block::NR_BITS) / (Block::NR_BITS*8));

        // Read bits blockwise:
        for(int w=0 ; w<numBlocks ; ++w) {
            blocks.push_back(Block());
            inputFile.read(reinterpret_cast<char*>(&blocks[w]), Block::NR_BITS); // We read one block (NR_BITS bytes) at the time 
        }

        // For counting how many values we have already read (so that we don't read the empty bits at the end) 
        // and added (so that we know where to put the alpha channel values):
        int readedValuesCounter = 0, addedValuesCounter = 0;

        for(int i=0 ; i<blocks.size() ; ++i) {  // For every block
            for(int j=0 ; j<8 ; ++j) {          // For every five bits (there are 8 fives in every block)
                if(readedValuesCounter == numValuesInFile) return true;

                sf::Uint8 valueFromBits = 0;

                for(int k=0 ; k<Block::NR_BITS ; ++k) { // For every bit
                    if(blocks[i].getBit(j*Block::NR_BITS + k)) valueFromBits += pow(2, Block::NR_BITS-1 - k);
                }

                // Scale value back to 8 bits:
                valueFromBits = (valueFromBits*255.0)/(pow(2.0, Block::NR_BITS)-1);
                readedValuesCounter++;

                int t = 1;
                if(grayscale) t = 3; // If grayscale is set we put there 3 identical values (one for each channel)

                for(int l=0 ; l<t ; ++l) {
                    pixelValues.push_back(valueFromBits);
                    addedValuesCounter++;
                }

                if((addedValuesCounter)%3 == 0) {
                    pixelValues.push_back(255); // Alpha component (sf::Image::saveToFile needs that)
                }
            }
        }
    }
    else { // TODO
        return false;

        if(codingType) {
            //BRun.Decompress();
        }
        else {
            //ACoding.Decompress();
        }
    }

    return true;
}


sf::Uint8* BinaryFile::getPxValuesAddress() {
    return &pixelValues[0];
}


unsigned BinaryFile::getW() {
    return imgW;    // uint16_t => unsigned int
}


unsigned BinaryFile::getH() {
    return imgH;    // uint16_t => unsigned int
}


