/*
  ==============================================================================

    BitCrusher.h
    Created: 24 Oct 2020 3:17:56pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "InterfaceDefines.h"

struct BitCrusher
{
    BitCrusher();
    ~BitCrusher() = default;
    
    void process(float* inputBuffer, int numberOfSamplesToRender, float sampleRateDivideFloat, float bitRatePower);
};
