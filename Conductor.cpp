/*
  ==============================================================================

    Conductor.cpp
    Created: 4 Oct 2020 1:28:21pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "Conductor.h"

void Conductor::update(AudioPlayHead::CurrentPositionInfo positionInfo, double sampleRate)
{
    _lengthOfOneBeatInSamples = (60/positionInfo.bpm) * sampleRate;
    _lengthOfOneBarInSamples = _lengthOfOneBeatInSamples * positionInfo.timeSigNumerator;
    _totalBeats = static_cast<int>(positionInfo.timeInSamples / _lengthOfOneBeatInSamples);
    
    if(positionInfo.timeSigDenominator != 0)
    {
        _currentBeat = (_totalBeats % positionInfo.timeSigDenominator) + 1;
        _currentBar = static_cast<int>(_totalBeats / positionInfo.timeSigDenominator) + 1;
    }
    else
    {
        _currentBeat = (_totalBeats % 4) + 1;
        _currentBar = static_cast<int>(_totalBeats / 4) + 1;
    }
    _bpm = positionInfo.bpm;
}
