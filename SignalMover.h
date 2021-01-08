/*
  ==============================================================================

    SignalMover.h
    Created: 28 Nov 2020 2:26:19pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "InterfaceDefines.h"

class SignalMover
{
public:
    SignalMover() = default;
    ~SignalMover() = default;
    
    void process(RangedAudioParameter* parameter, float&& incrementValue, double& minValue, double& maxValue);
    
private:
    bool _reverse {false};
};
