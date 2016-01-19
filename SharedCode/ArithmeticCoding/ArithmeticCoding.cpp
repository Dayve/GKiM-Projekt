#include "ArithmeticCoding.hpp"
#include <iostream>
#include <cmath>
using namespace std;


void ArithmeticCoding::Compress() {
    float lowerBound = 0.0f;
    for(int i=0, j=0 ; i<ScaledValues.size() ; ++i, ++j) {
        if(j == N_MULT) {
            Results.push_back(lowerBound);
            j = 0;
            lowerBound = 0.0f;
        }
        lowerBound += ScaledValues[i] * pow(PROB, j+1);
    }
    // TODO: Check & finish
}


void ArithmeticCoding::Decompress() {
}

