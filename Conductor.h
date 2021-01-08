/*
  ==============================================================================

    Conductor.h
    Created: 4 Oct 2020 1:28:21pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

struct Conductor
{
    Conductor() = default;
    ~Conductor() = default;
    
    void update(AudioPlayHead::CurrentPositionInfo positionInfo, double sampleRate);
    
    int _lengthOfOneBeatInSamples {0};
    int _lengthOfOneBarInSamples {0};
    int _currentBeat {0};
    int _currentBar {0};
    int _totalBeats {0};
    int _sampleCount {0};
    int _beatCount {0};
    int _bpm {0};
};
