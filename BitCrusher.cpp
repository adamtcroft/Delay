/*
  ==============================================================================

    BitCrusher.cpp
    Created: 24 Oct 2020 3:17:56pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "BitCrusher.h"

BitCrusher::BitCrusher()
{
    
}

void BitCrusher::process(float* inputBuffer,
                         int numberOfSamplesToRender,
                         float sampleRateDivideFloat,
                         float bitRatePower)
{
    int sampleRateDivide = jmap(sampleRateDivideFloat, 125.f, 1.f);
    bitRatePower = jmap(bitRatePower, 2.f, 24.f);
        
    float quantizationAmt = powf(2, bitRatePower);
    
    for(int i = 0; i < numberOfSamplesToRender; i++)
    {
        float remainder = fmodf(inputBuffer[i], 1/quantizationAmt);
        inputBuffer[i] = inputBuffer[i] - remainder;

        if(sampleRateDivide > 1)
        {
            // multiplied by .75 to reduce loudness
            if(i % sampleRateDivide != 0)
                inputBuffer[i] = (inputBuffer[i - i % sampleRateDivide]) * .3;
        }
    }
}
