#ifndef _BYTERUN_H_
#define _BYTERUN_H_

#include <SFML/Graphics.hpp>
#include <vector>

// TODO: /Refactoring/ Add inheritance

class ByteRun {
    public:
        void Compress();
        void Decompress();

        void Test(); // TODO: Remove

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<char> Results;              // Output sequence of the algorithm

        void* GetResultsAddr();
};

#endif

