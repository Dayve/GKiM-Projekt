#include "ArithmeticCoding.hpp"
using namespace std;


void ArithmeticCoding::Compress() {
    float lowerBound = 0.0f, upperBound = PROB;

    for(vector<sf::Uint8>::size_type i=0, j=0 ; i<ScaledValues.size()+1 /*+1 because of writing (L15)*/ ; ++i, ++j) {
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
    float upperBound = PROB, lowerBound = 0.0f;
    sf::Uint8 decodedValue = 0;

    // Number of nested for loop iterations: (used when last float represents less than SEQ_LEN numbers)
    unsigned numReps = nValues % SEQ_LEN;

    for(vector<float>::size_type i=0 ; i<Results.size() ; ++i) {
        bool lastPass = (i == Results.size()-1);

        for(unsigned n=1 ; n < (lastPass ? numReps : SEQ_LEN+1) ; ++n) {
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

