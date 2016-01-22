#ifndef _BYTERUN_H_
#define _BYTERUN_H_

#include <SFML/Graphics.hpp>
#include <vector>

class ByteRun {
    public:
        void Compress();
        void Decompress();

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<char> Results;              // Output sequence of the algorithm

        void* GetResultsAddr();
};

#endif

