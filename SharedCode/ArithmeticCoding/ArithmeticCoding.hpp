#ifndef _ARITHMETIC_CODING_HPP_
#define _ARITHMETIC_CODING_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

class ArithmeticCoding {
    public:
        void Compress();
        void Decompress(uint32_t nValues);

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<double> Results;             // Output sequence of the algorithm

        void* GetResultsAddr();

    private:
        constexpr static double PROB = 1.0f/32.0f;   // Fixed probability
        const static unsigned SEQ_LEN = 10;          // Length of sequence per double
};

#endif
