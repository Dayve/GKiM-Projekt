#include "ArithmeticCoding.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void ArithmeticCoding::Test() { // Passed every time for SEQ_LEN = 4
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);

    for(int i=0 ; i<ScaledValues.size() ; ++i) {
        cout << static_cast<unsigned short>(ScaledValues[i]) << " ";
    }
    cout << endl;

    Results.clear();
    Compress();
    
    for(int i=0 ; i<Results.size() ; ++i) {
        cout << setprecision(8) << Results[i] << endl;
    }

    ScaledValues.clear();
    Decompress();

    for(int i=0 ; i<ScaledValues.size() ; ++i) {
        cout << static_cast<unsigned short>(ScaledValues[i]) << " ";
    }
    cout << endl;
}


void ArithmeticCoding::Compress() {
    // Empty "Results" vector
    // Data in "ScaledValues" vector

    float lowerBound = 0.0f, upperBound = PROB;

    for(int i=0, j=0 ; i<ScaledValues.size()+1 /*+1 because of writing (L45)*/ ; ++i, ++j) {
        bool lastPass = (i == ScaledValues.size());

        if(j == SEQ_LEN or lastPass) {
            Results.push_back((lowerBound + upperBound)/2.0f);
            j = 0;
            lowerBound = 0.0f, upperBound = PROB;
        }

        lowerBound += ScaledValues[i] * pow(PROB, j+1);
        upperBound = lowerBound + pow(PROB, j+1);
    }
}


void ArithmeticCoding::Decompress() {
    // Empty "ScaledValues" vector
    // Data in "Results" vector

    float upperBound = PROB, lowerBound = 0.0f;
    sf::Uint8 decodedValue = 0;

    for(int i=0 ; i<Results.size() ; ++i) {
        for(int n=1 ; n<(SEQ_LEN+1) ; ++n) {
            while(not (Results[i] > lowerBound and Results[i] < lowerBound+upperBound)) {
                lowerBound += pow(PROB, n);
                decodedValue += 1;
            }

            ScaledValues.push_back(decodedValue);
            decodedValue = 0;

            upperBound *= PROB;
        }

        upperBound = PROB, lowerBound = 0.0f;
        decodedValue = 0;
    }
}



