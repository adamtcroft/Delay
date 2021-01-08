/*
  ==============================================================================

    Gain.h
    Created: 24 Aug 2020 9:52:20pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class Gain
{
public:
    Gain() = default;
    ~Gain() = default;
    
    void process(float* inputAudio,
                 float inputGainLevel,
                 float* ouptutAudio,
                 int numberOfSamplesToRender);
};
