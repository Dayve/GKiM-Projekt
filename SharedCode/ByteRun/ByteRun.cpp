#include "ByteRun.hpp"

// ByteRun algorithm, basic (and not really adapted yet) version.
// Compresses sequences of at least two identical bytes, prints results to STDOUT.

#include <string>
#include <iostream>
using namespace std;


void ByteRun::Compress(sf::Uint8 sequence[], int length) {
    int i = 0;

    // Until every byte is compressed:
    while (i < length)
    {
        // Sequence of repeated bytes:
        if ((i < length-1) and (sequence[i] == sequence[i+1])) {
            // Calculate the length of a sequence:
            int j = 0;
            while ((i+j < length-1) and (sequence[i+j] == sequence[i+j+1]) and (j < 127)) j++;

            // Print out compressed sequence:
            cout << -j << ", " << (int)sequence[i+j] << ", ";

            // Move the pointer by the length of a sequence:
            i += (j+1);
        }
        // Sequence of different bytes:
        else {
            // Calculate the length of a sequence:
            int j=0;
            while ((i+j < length-1) and (sequence[i+j] != sequence[j+i+1]) and (j < 128)) j++;

            // And ending:
            if ((i+j == length-1) and (j < 128)) j++;

            // Print out compressed sequence:
            cout << (j-1) << ", ";
            for (int k=0; k<j; k++) {
                cout << (int)sequence[i+k] << ", ";
            }

            // Move the pointer by the length of a sequence:
            i += j;
        }
    }
}


void ByteRun::Test() { // Can be removed
    sf::Uint8 t[] = {1,2,3,4,4,4,6,6,9,9,9,9,9};
    Compress(t, sizeof(t)/sizeof(t[0]));
    // Result: 2, 1, 2, 3, -2, 4, -1, 6, -4, 9
}


void ByteRun::Decompress(sf::Uint8 sequence[], int length) {
    int i = 0;

    // Until every byte is decompressed:
    while (i < length) {
        // Code for "empty":
        if (sequence[i] == -128) i++;

        // Sequence of repeated bytes:
        else if (sequence[i] < 0) {
            for (int j=0; j<-(sequence[i]-1); j++) {
                cout << (int)sequence[i+1] << ", ";
            }
            i += 2;
        }
        // Sequence of different bytes:
        else {
            for (int j=0; j<(sequence[i]+1); j++) {
                cout << (int)sequence[i+1+j] << ", ";
            }
            i += sequence[i]+2;
        }
    }
}

