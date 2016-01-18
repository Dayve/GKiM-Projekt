#include "ArithmeticCoding.hpp"
#include <iostream>
using namespace std;

ArithmeticCoding::ArithmeticCoding() {
    float probVal = 0.0f;
    for(sf::Uint8 pxVal=0 ; pxVal<32 ; ++pxVal) {
        ProbTable[pxVal] = bounds(probVal, probVal+PROB);
        probVal += PROB;
    }

    /*
    for(sf::Uint8 pxVal=0 ; pxVal<32 ; ++pxVal) {
        cout << static_cast<unsigned short>(pxVal) << ": " << ProbTable[pxVal].lower << " > " << ProbTable[pxVal].upper << endl;
    }
    // Works fine
    */
}


void ArithmeticCoding::Compress() {
}


void ArithmeticCoding::Decompress() {
}

