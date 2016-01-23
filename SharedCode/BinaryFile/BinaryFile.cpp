#include "BinaryFile.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;


void BinaryFile::FetchValuesFromImg(sf::Image& image) {
    cout << " -> Fetching values from an image...\n";

    for(unsigned int yy=0 ; yy<image.getSize().y ; ++yy) {
        for(unsigned int xx=0 ; xx<image.getSize().x ; ++xx) {
            pixelValues.push_back(image.getPixel(xx, yy).r);
            pixelValues.push_back(image.getPixel(xx, yy).g);
            pixelValues.push_back(image.getPixel(xx, yy).b);
        }
    }
}


bool BinaryFile::ExportFromImg(sf::Image& image, unsigned char codingType, bool grayscale, const string& resultPathWithName) {
    FetchValuesFromImg(image);

    vector<bool> bitBuffer;  // Holds the results of converting the pixel values (scaled down to fit in 5 bits) from decimal to binary

    // We scale the pixel values down to 5 bits and put results in the bitBuffer:
    // (We've put for loops inside if blocks and not the other way around, because this way we check condidion only once.
    // Probably we should do something similar with nested switch, but it would increase the amount of code drastically.)

    cout << " -> Scaling values...\n";

    if(grayscale) {
        for(vector<sf::Uint8>::size_type i=0 ; i<pixelValues.size() ; i += 3) {
            sf::Uint8 avgColor = (pixelValues[i] + pixelValues[i+1] + pixelValues[i+2])/3;  // Calculate the grayscale equivalent of a given color
            sf::Uint8 scaledVal = (avgColor * (pow(2.0, Block::NR_BITS)-1))/255.0;          // Scale down to 5 bits in the temporary variable
            
            switch(codingType) {
                case 2: // (Scaling to 5-bit values)
                    for(int w=0 ; w<Block::NR_BITS ; ++w) {
                        bitBuffer.push_back(scaledVal % 2);
                        scaledVal /= 2;
                    }
                    break;

                case 1: // Byterun
                    BRun.ScaledValues.push_back(scaledVal);
                    break;

                case 0: // Arithmetic Coding
                    ACoding.ScaledValues.push_back(scaledVal);
                    break;
            }
        }
    }
    else {
        for(auto pxValue : pixelValues) {
            sf::Uint8 scaledVal = (pxValue * (pow(2.0, Block::NR_BITS)-1))/255.0;    // Scale down to 5 bits in the temporary variable

            switch(codingType) {
                case 2: // (Scaling to 5-bit values)
                    for(int w=0 ; w<Block::NR_BITS ; ++w) {
                        bitBuffer.push_back(scaledVal % 2);
                        scaledVal /= 2;
                    }
                    break;

                case 1: // Byterun
                    BRun.ScaledValues.push_back(scaledVal);
                    break;

                case 0: // Arithmetic Coding
                    ACoding.ScaledValues.push_back(scaledVal);
                    break;
            }
        }
    }

    imgW = image.getSize().x;
    imgH = image.getSize().y;
    uint32_t numValuesInFile = 0;

    cout << " -> Compressing...\n";

    if(codingType == 2) {
        // Every NR_BITS bits in bitBuffer are in reverse order, so we fix that: (TODO: could be integrated with /case 2/)
        for(vector<bool>::size_type i=0 ; i<bitBuffer.size()/Block::NR_BITS ; ++i) {
            // NR_BITS/2 will be rounded down for odd numbers, but this is fine, because the middle bit won't be swapped anyway:
            for(int j=0 ; j<Block::NR_BITS/2 ; ++j) swap(bitBuffer[j + i*Block::NR_BITS], bitBuffer[Block::NR_BITS-1-j + i*Block::NR_BITS]);
        }

        blocks.push_back(Block());  // There were no Blocks before in blocks vector, so we add the first one

        // Here we put bits from bitBuffer into blocks of 40 bits (5*8 bytes):
        for(vector<bool>::size_type i=0, j=0, blockIndex=0 ; i<bitBuffer.size() ; ++i, ++j) {
            if(j > (Block::NR_BITS*8)-1) {
                j -= Block::NR_BITS*8;
                blocks.push_back(Block());
                blockIndex++;
            }
            if(bitBuffer[i]) blocks[blockIndex].setBit(j);
        }

        numValuesInFile = grayscale ? imgW*imgH : imgW*imgH*3;
    }
    else {
        if(codingType) {
            BRun.Compress(grayscale);
            numValuesInFile = BRun.Results.size();
        }
        else {
            ACoding.Compress();
            numValuesInFile = ACoding.Results.size();
        }
    }

    /* ----------------------------------------------------------------------------------------
    Header of the binary file: (9B)
     1) 2 Bytes for width of an image
     2) 2 Bytes for height of an image
     2) 4 Bytes for the amount of values (respectively: floats, chars or 5-bit sequences)
     3) 1 Byte for coding type and grayscale info:
      a) Grayscale - on most significant bit (0 colored, 1 grayscale)
      b) Coding type - as a number (0 Arithmetic Coding, 1 Byterun, 2 scaling to 5-bit values)

     We can later deduce the number of blocks/pixels from width and height or number of values.
     --------------------------------------------------------------------------------------- */

    ofstream outputFile(resultPathWithName.c_str(), ios::binary | ios::out);
    if(outputFile.fail()) return false;

    unsigned char codingAndGrayscaleData = codingType;

    if(grayscale) codingAndGrayscaleData |= 0b10000000;     // C++14
    //if(grayscale) codingAndGrayscaleData += pow(2, 7);        // Older

    cout << " -> Writing binary file...\n";

    // Writing header:
    outputFile.write(reinterpret_cast<const char*>(&imgW), sizeof(imgW));
    outputFile.write(reinterpret_cast<const char*>(&imgH), sizeof(imgH));
    outputFile.write(reinterpret_cast<const char*>(&numValuesInFile), sizeof(numValuesInFile));
    outputFile.write(reinterpret_cast<const char*>(&codingAndGrayscaleData), sizeof(codingAndGrayscaleData));

    // Writing values:
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
    return true;
}



bool BinaryFile::ImportFromFile(const std::string& pathWithName) {
    ifstream inputFile(pathWithName.c_str(), ios::binary | ios::in);
    if(inputFile.fail()) return false;

    cout << " -> Binary file " << pathWithName << " successfully loaded\n";

    bool grayscale = false;
    unsigned char codingType;
    uint32_t numValuesInFile;

    // Read the header:
    inputFile.read(reinterpret_cast<char*>(&imgW), sizeof(imgW));
    inputFile.read(reinterpret_cast<char*>(&imgH), sizeof(imgH));

    inputFile.read(reinterpret_cast<char*>(&numValuesInFile), sizeof(numValuesInFile));

    inputFile.read(reinterpret_cast<char*>(&codingType), sizeof(codingType));

    if(codingType >= pow(2,7)) { // Grayscale option on MSB of that byte
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
    cout << " Image dimensions: " << imgW << "x" << imgH << endl;
    cout << "--------------------------------------\n";

    cout << " -> Reading data & decompressing...\n";

    if (codingType == 2) {
        // Calculate the number of blocks based of the amount of the values in a file:
		// (If the amount of bits fits in an integral number of Blocks, then we get this number, if not, then we add one Block at the end)
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
        uint32_t readedValuesCounter = 0, addedValuesCounter = 0;

        for(auto block : blocks) { 		// For every block
            for(int j=0 ; j<8 ; ++j) {  // For every five bits (there are 8 fives in every block)
                if(readedValuesCounter == numValuesInFile) return true;

                sf::Uint8 valueFromBits = 0;

                for(int k=0 ; k<Block::NR_BITS ; ++k) {  // For every bit
                    if(block.getBit(j*Block::NR_BITS + k)) valueFromBits += pow(2, Block::NR_BITS-1 - k);
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
    else {                                          // TODO: /Refactoring/ DRY
        if(codingType) { // Byterun
            for(uint32_t p=0 ; p<numValuesInFile ; ++p) {
                char resultFromFile;
                inputFile.read(&resultFromFile, sizeof(resultFromFile));
                BRun.Results.push_back(resultFromFile);
            }

            BRun.Decompress(grayscale);

            int addedValuesCounter = 0;
            for(auto sValue : BRun.ScaledValues) {
                sf::Uint8 scaledBack = (sValue * 255.0)/(pow(2.0, Block::NR_BITS)-1);

                int t = 1;
                if(grayscale) t = 3; // If grayscale is set we put there 3 identical values (one for each channel)

                for(int l=0 ; l<t ; ++l) {
                    pixelValues.push_back(scaledBack);
                    addedValuesCounter++;
                }

                if((addedValuesCounter)%3 == 0) {
                    pixelValues.push_back(255); // Alpha component (sf::Image::saveToFile needs that)
                }
            }
        }
        else { // Arithmetic Coding
            for(uint32_t p=0 ; p<numValuesInFile ; ++p) {
                float resultFromFile;
                inputFile.read(reinterpret_cast<char*>(&resultFromFile), sizeof(resultFromFile));
                ACoding.Results.push_back(resultFromFile);
            }

            // We forward the amount of values that should be decompressed:
            ACoding.Decompress(grayscale ? imgW*imgH : imgW*imgH*3); 

            int addedValuesCounter = 0;
            for(auto sValue : ACoding.ScaledValues) {
                sf::Uint8 scaledBack = (sValue * 255.0)/(pow(2.0, Block::NR_BITS)-1);

                int t = 1;
                if(grayscale) t = 3; // If grayscale is set we put there 3 identical values (one for each channel)

                for(int l=0 ; l<t ; ++l) {
                    pixelValues.push_back(scaledBack);
                    addedValuesCounter++;
                }

                if((addedValuesCounter)%3 == 0) {
                    pixelValues.push_back(255); // Alpha component (sf::Image::saveToFile needs that)
                }
            }
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


