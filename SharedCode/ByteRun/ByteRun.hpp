#ifndef _BYTERUN_H_
#define _BYTERUN_H_

#include <SFML/Graphics.hpp>

class ByteRun {
    public:
        static void Compress(sf::Uint8 sequence[], int length);
        static void Decompress(sf::Uint8 sequence[], int length);

        static void Test();
};

#endif

