#include "ByteRun.hpp"
using namespace std;


void ByteRun::Compress(bool valuesAreInGrayscale) {
    if(valuesAreInGrayscale) { // Then we compress by single values
        vector<sf::Uint8>::size_type length = ScaledValues.size(), i=0;
        unsigned j;

        // Until every byte is compressed:
        while (i < length) {
            // Sequence of repeated bytes:
            if((i < length-1) and (ScaledValues[i] == ScaledValues[i+1])) {
                // Calculate the length of a sequence:
                j = 0;
                while((i+j < length-1) and (ScaledValues[i+j] == ScaledValues[i+j+1]) and (j < 127)) j++;

                // Save compressed sequence:
                Results.push_back(static_cast<char>(-j));
                Results.push_back(static_cast<char>(ScaledValues[i+j]));

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
                Results.push_back(static_cast<char>(j-1));

                for (unsigned k=0 ; k<j ; k++) {
                    Results.push_back(static_cast<char>(ScaledValues[i+k]));
                }

                // Move the pointer by the length of a sequence:
                i += j;
            }
        }
        ScaledValues.clear(); // Not needed anymore
    }
    else { // We compress by 3s of values (whole colors)
        vector<sf::Color> scaledPixels;
        unsigned j;

        // Get scaled values and put them into sf::Colors: (FIXME: takes a lot of memory, again, could be integrated)
        for(vector<sf::Uint8>::size_type it=0 ; it<ScaledValues.size() ; it += 3) {
            scaledPixels.push_back(sf::Color(ScaledValues[it], ScaledValues[it+1], ScaledValues[it+2]));
        }

        ScaledValues.clear(); // Not needed anymore
        vector<sf::Color>::size_type length = scaledPixels.size(), i=0;

        // Until every color is compressed:
        while (i < length) {
            // Sequence of repeated colors:
            if((i < length-1) and (scaledPixels[i] == scaledPixels[i+1])) {
                // Calculate the length of a sequence:
                j = 0;
                while((i+j < length-1) and (scaledPixels[i+j] == scaledPixels[i+j+1]) and (j < 127)) j++;

                // Save compressed sequence:
                Results.push_back(static_cast<char>(-j));
                Results.push_back(static_cast<char>(scaledPixels[i+j].r));
                Results.push_back(static_cast<char>(scaledPixels[i+j].g));
                Results.push_back(static_cast<char>(scaledPixels[i+j].b));

                // Move the pointer by the length of a sequence:
                i += (j+1);
            }
            // Sequence of different colors:
            else {
                // Calculate the length of a sequence:
                j=0;
                while ((i+j < length-1) and (scaledPixels[i+j] != scaledPixels[j+i+1]) and (j < 128)) j++;

                // And ending:
                if ((i+j == length-1) and (j < 128)) j++;

                // Save compressed sequence:
                Results.push_back(static_cast<char>(j-1));

                for (unsigned k=0 ; k<j ; k++) {
                    Results.push_back(static_cast<char>(scaledPixels[i+k].r));
                    Results.push_back(static_cast<char>(scaledPixels[i+k].g));
                    Results.push_back(static_cast<char>(scaledPixels[i+k].b));
                }

                // Move the pointer by the length of a sequence:
                i += j;
            }
        }
    }
}


void ByteRun::Decompress(bool valuesAreInGrayscale) {
    vector<char>::size_type length = Results.size(), i=0;

    // Until every byte/color is decompressed:
    while (i < length) {
        // Code for "empty":
        if (Results[i] == -128) i++;

        // Sequence of repeated bytes/colors:
        else if (Results[i] < 0) {
            for (int j=0 ; j < -(Results[i]-1) ; j++) {
                ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+1]));

                if(!valuesAreInGrayscale) {
                    ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+2]));
                    ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+3]));
                }
            }
            // Move the pointer:
            i += valuesAreInGrayscale ? 2 : 4;
        }
        // Sequence of different bytes/colors:
        else {
            int jump = (valuesAreInGrayscale ? 1 : 3); // 1 value for a color for grayscale, 3 for normal

            for (int j=0 ; j<(Results[i]*jump + 1) ; j+=jump) {
                ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+1+j]));

                if(!valuesAreInGrayscale) {
                    ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+2+j]));
                    ScaledValues.push_back(static_cast<sf::Uint8>(Results[i+3+j]));
                }
            }
            // Move the pointer:
            i += (valuesAreInGrayscale ? (Results[i]+2) : ((Results[i]+1)*3 + 1));
        }
    }

    Results.clear();
}


void* ByteRun::GetResultsAddr() {
    return Results.empty() ? nullptr : &Results[0];
}

