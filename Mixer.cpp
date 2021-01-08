/*
  ==============================================================================

    Mixer.cpp
    Created: 5 Dec 2020 3:31:13pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "Mixer.h"

void Mixer::process(float* outputBuffer, float* signalToMix, float wetDryParameter, int blockSize)
{
    float wet = wetDryParameter;
    float dry = 1.f - wetDryParameter;
    
    for(int sample = 0; sample < blockSize; sample++)
    {
        outputBuffer[sample] = (outputBuffer[sample] * dry) + (signalToMix[sample] * wet);
    }
}
