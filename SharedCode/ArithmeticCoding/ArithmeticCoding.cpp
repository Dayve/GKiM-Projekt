#include "ArithmeticCoding.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void ArithmeticCoding::Test() { // TODO: Remove
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);
    ScaledValues.push_back(rand()%32);

    for(vector<sf::Uint8>::size_type i=0 ; i<ScaledValues.size() ; ++i) {
        cout << static_cast<unsigned short>(ScaledValues[i]) << " ";
    }
    cout << endl;

    Results.clear();
    Compress();
    
    for(vector<float>::size_type i=0 ; i<Results.size() ; ++i) {
        cout << setprecision(8) << Results[i] << endl;
    }

    ScaledValues.clear();
    Decompress(ScaledValues.size());

    for(vector<sf::Uint8>::size_type i=0 ; i<ScaledValues.size() ; ++i) {
        cout << static_cast<unsigned short>(ScaledValues[i]) << " ";
    }
    cout << endl;
}


void ArithmeticCoding::Compress() {
    // Empty "Results" vector
    // Data in "ScaledValues" vector

    float lowerBound = 0.0f, upperBound = PROB;

    for(vector<sf::Uint8>::size_type i=0, j=0 ; i<ScaledValues.size()+1 /*+1 because of writing (L48)*/ ; ++i, ++j) {
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


void ArithmeticCoding::Decompress(uint32_t nValues) {
    // Empty "ScaledValues" vector
    // Data in "Results" vector

    // TODO: Add support for series of values (at the end) shorter than SEQ_LEN, using nValues

    float upperBound = PROB, lowerBound = 0.0f;
    sf::Uint8 decodedValue = 0;

    for(vector<float>::size_type i=0 ; i<Results.size() ; ++i) {
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


void* ArithmeticCoding::GetResultsAddr() {
    return Results.empty() ? nullptr : &Results[0];
}

