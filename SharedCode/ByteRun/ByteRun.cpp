#include "ByteRun.hpp"
using namespace std;


void ByteRun::Compress() {
    vector<sf::Uint8>::size_type length = ScaledValues.size(), i=0, j=0;

    // Until every byte is compressed:
    while (i < length)
    {
        // Sequence of repeated bytes:
        if ((i < length-1) and (ScaledValues[i] == ScaledValues[i+1])) {
            // Calculate the length of a sequence:
            j = 0;
            while ((i+j < length-1) and (ScaledValues[i+j] == ScaledValues[i+j+1]) and (j < 127)) j++;

            // Save compressed sequence:
            Results.push_back(-j);
            Results.push_back((char)ScaledValues[i+j]);

            // Move the pointer by the length of a sequence:
            i += (j+1);
        }
        // Sequence of different bytes:
        else {
            // Calculate the length of a sequence:
            j=0;
            while ((i+j < length-1) and (ScaledValues[i+j] != ScaledValues[j+i+1]) and (j < 128)) j++;

            // And ending:
            if ((i+j == length-1) and (j < 128)) j++;

            // Save compressed sequence:
            Results.push_back(j-1);
            for (vector<sf::Uint8>::size_type k=0; k<j; k++) {
                Results.push_back((char)ScaledValues[i+k]);
            }

            // Move the pointer by the length of a sequence:
            i += j;
        }
    }
}


void ByteRun::Decompress() {
    int i = 0;
    int length = Results.size();

    // Until every byte is decompressed:
    while (i < length) {
        // Code for "empty":
        if (Results[i] == -128) i++;

        // Sequence of repeated bytes:
        else if (Results[i] < 0) {
            for (int j=0; j<-(Results[i]-1); j++) {
                ScaledValues.push_back((sf::Uint8)Results[i+1]);
            }
            i += 2;
        }
        // Sequence of different bytes:
        else {
            for (int j=0; j<(Results[i]+1); j++) {
                ScaledValues.push_back((sf::Uint8)Results[i+1+j]);
            }
            i += Results[i]+2;
        }
    }
}


void* ByteRun::GetResultsAddr() {
    return Results.empty() ? nullptr : &Results[0];
}

