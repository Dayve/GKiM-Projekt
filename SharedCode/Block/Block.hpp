#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdint.h>

class Block {
	public:
		Block() : bytes{0} {}
		~Block() {};

		static const int NR_BITS = 5;

		uint8_t bytes[NR_BITS];

		void setBit(int whichBit);
		bool getBit(int whichBit);

		void* getBytesAddr();
};

#endif
