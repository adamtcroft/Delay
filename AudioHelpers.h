/*
  ==============================================================================

    AudioHelpers.h
    Created: 24 Aug 2020 9:51:29pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once

#define parameterSmoothingCoefficient 0.4f
#define feedbackSmoothingCoefficient 0.4f

inline float linearInterp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

inline void wetDryMix(float* outputBuffer, float* signalToMix, float&& wetDryParameter, int&& blockSize)
{
    float wet = wetDryParameter;
    float dry = 1.f - wetDryParameter;
    
    for(int sample = 0; sample < blockSize; sample++)
    {
        outputBuffer[sample] = (outputBuffer[sample] * dry) + (signalToMix[sample] * wet);
    }
}

inline void insertFeedback(float* outputBuffer, float* feedbackBuffer, float& feedbackParameter, int&& blockSize)
{
    for(int sample = 0; sample < blockSize; sample++)
    {
        outputBuffer[sample] = outputBuffer[sample] + (feedbackBuffer[sample] * feedbackParameter);
    }
}

inline void copyChannel(float* source, float* destination, int numSamplesToCopy)
{
    for(int i = 0; i < numSamplesToCopy; i++)
    {
        destination[i] = source[i];
    }
}
