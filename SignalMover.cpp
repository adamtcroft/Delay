/*
  ==============================================================================

    SignalMover.cpp
    Created: 28 Nov 2020 2:26:19pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "SignalMover.h"

void SignalMover::process(RangedAudioParameter* parameter, float&& incrementValue, double& minValue, double& maxValue)
{    
    float parameterIncrementValue = jmap(incrementValue, 0.f, 0.01f);

    if(parameter->convertFrom0to1(parameter->getValue()) <= minValue && _reverse == false)
        _reverse = true;

    if(parameter->convertFrom0to1(parameter->getValue()) >= maxValue && _reverse == true)
        _reverse = false;
    
    if(_reverse == false)
        parameter->setValue(parameter->getValue() - parameterIncrementValue);
    else
        parameter->setValue(parameter->getValue() + parameterIncrementValue);

}
