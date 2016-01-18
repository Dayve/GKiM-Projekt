#ifndef _BYTERUN_H_
#define _BYTERUN_H_

#include <SFML/Graphics.hpp>
#include <vector>

// TODO: Refactoring: add inheritance

class ByteRun {
    public:
        void Compress();
        void Decompress();

        void Test(); // TODO: REMOVE WHEN NOT NEEDED ANYMORE

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<char> Results;              // Output sequence of the algorithm
};

#endif

