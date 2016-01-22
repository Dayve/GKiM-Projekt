#ifndef _ARITHMETIC_CODING_HPP_
#define _ARITHMETIC_CODING_HPP_

#include <map>
#include <SFML/Graphics.hpp>

// TODO: /Refactoring/ Add inheritance

class ArithmeticCoding {
    public:
        void Compress();
        void Decompress(uint32_t nValues);

        void Test(); // TODO: Remove

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<float> Results;             // Output sequence of the algorithm

        void* GetResultsAddr();

    private:
        constexpr static float PROB = 1.0f/32.0f;   // Fixed probability
        const static int SEQ_LEN = 4;               // Length of sequence per float
};

#endif
