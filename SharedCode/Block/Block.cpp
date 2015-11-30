#include "Block.hpp"
#include <cmath>

void Block::setBit(int whichBit) {
	if(whichBit > 39) return;			// whichBit can range from 0 to 39

	int posInByte = whichBit % 8;
	int whichByte = whichBit / 8;

	uint8_t temp = pow(2, 7-posInByte);	// Set 1 at (7-posInByte)th position in temp byte

	bytes[whichByte] |= temp;			// "or" together temp byte and corresponding byte from array
}


bool Block::getBit(int whichBit) {
	// TODO
	return false;
}


void* Block::getBytesAddr() {
	return &bytes[0];
}

