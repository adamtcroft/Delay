/*
  ==============================================================================

    Mixer.h
    Created: 5 Dec 2020 3:31:13pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

struct Mixer
{
    Mixer() = default;
    ~Mixer() = default;
    
    void process(float* outputBuffer,
                 float* signalToMix,
                 float wetDryParameter,
                 int blockSize);
};
