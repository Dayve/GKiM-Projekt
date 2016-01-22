#include "Block.hpp"
#include <cmath>
#include <bitset>
using namespace std;

void Block::setBit(int whichBit) {
    if(whichBit < 0 or whichBit > (NR_BITS*8 - 1)) return; // whichBit can range from 0 to 39

    int posInByte = whichBit % 8;
    int whichByte = whichBit / 8;

    uint8_t temp = pow(2, 7-posInByte); // Set 1 at the (7-posInByte)th position in temp byte

    bytes[whichByte] |= temp;           // "OR" together the temp byte and the corresponding byte from array
}


bool Block::getBit(int whichBit) {
    if(whichBit < 0 or whichBit > (NR_BITS*8 - 1)) return false; // whichBit can range from 0 to 39

    bitset<8> setFromBytes[NR_BITS];
    for(int a=0 ; a<NR_BITS ; ++a) setFromBytes[a] = bytes[a];

    int posInByte = whichBit % 8;
    int whichByte = whichBit / 8;

    return setFromBytes[whichByte][7-posInByte];
}


void* Block::getBytesAddr() {
    return &bytes[0];
}

