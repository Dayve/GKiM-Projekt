#ifndef _ARITHMETIC_CODING_HPP_
#define _ARITHMETIC_CODING_HPP_

#include <map>
#include <SFML/Graphics.hpp>

// TODO: Refactoring: add inheritance

class ArithmeticCoding {
    public:
        void Compress();
        void Decompress();

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<float> Results;             // Output sequence of the algorithm

    private:
        constexpr static float PROB = 1.0f/32.0f;   // Fixed probability
        const static int N_MULT = 24;
};

#endif
