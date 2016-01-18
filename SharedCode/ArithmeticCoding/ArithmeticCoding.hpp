#ifndef _ARITHMETIC_CODING_HPP_
#define _ARITHMETIC_CODING_HPP_

#include <map>
#include <SFML/Graphics.hpp>

// TODO: Refactoring: add inheritance

class ArithmeticCoding {
    public:
        ArithmeticCoding();

        void Compress();
        void Decompress();

        std::vector<sf::Uint8> ScaledValues;    // The algorithm will operate on those
        std::vector<float> Results;             // Output sequence of the algorithm

    private:
        constexpr static float PROB = 1.0f/32.0f;   // Fixed probability
        const static int MAX_N_MULT = 28;           // Not sure if it's right [FIXME]

        struct bounds {
            float lower, upper;

            bounds() {}
            bounds(float l, float u) : lower(l), upper(u) {}
        };

        std::map<sf::Uint8, bounds> ProbTable;
};

#endif
