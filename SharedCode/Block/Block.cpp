#include "Block.hpp"
#include <cmath>
#include <bitset>
#include <iostream>
using namespace std;

void Block::setBit(int whichBit) {
	if(whichBit > 39) return;			// whichBit can range from 0 to 39

	int posInByte = whichBit % 8;
	int whichByte = whichBit / 8;

	uint8_t temp = pow(2, 7-posInByte);	// Set 1 at (7-posInByte)th position in temp byte

	bytes[whichByte] |= temp;			// "or" together temp byte and corresponding byte from array
}


bool Block::getBit(int whichBit) {
	bitset<8> setFromBytes[NR_BITS];
	for(int a=0 ; a<NR_BITS ; ++a) setFromBytes[a] = bytes[a];

	int posInByte = whichBit % 8;
	int whichByte = whichBit / 8;

	return setFromBytes[whichByte][posInByte];	// TODO: MAKE SURE IT'S CORRECT TO USE [][]
}


void* Block::getBytesAddr() {
	return &bytes[0];
}

