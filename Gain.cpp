/*
  ==============================================================================

    Gain.cpp
    Created: 24 Aug 2020 9:52:20pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "Gain.h"

void Gain::process(float* inputAudio,
                   float inputGainLevel,
                   float* outputAudio,
                   int numberOfSamplesToRender)
{
    float gainMapped = jmap(inputGainLevel, 0.0f, 1.0f, -24.f, 24.f);
    gainMapped = Decibels::decibelsToGain(gainMapped, -24.f);
    
    for (int i = 0; i < numberOfSamplesToRender; i++)
    {
        outputAudio[i] = inputAudio[i] * inputGainLevel;
    }
}
