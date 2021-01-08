/*
  ==============================================================================

    Delay.h
    Created: 24 Aug 2020 9:52:27pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AudioHelpers.h"
#include <math.h>

class Delay
{
public:
    Delay();
    ~Delay() = default;
    
    void setSampleRate(double sampleRate);
    void setBPM(int bpm);
    void reset(double sampleRate);
    void process(float* inputAudio,
                 float delayTime,
//                 float feedbackAmount,
                 float wetDry,
                 int delayState,
                 float* outputAudio,
                 int numberOfSamplesToRender);
    
protected:
    double getInterpolatedSample(float delayTimeInSamples);
    int getDelaySyncedTime(float delayTime);
    int getDelayFreeTime(double delayTime);
    
    double _feedbackSample;
    float _timeSmoothed, _feedbackSmoothed, _feedbackTargetValue, _delayTimeTargetValue;
    int _sampleRate, _delayIndex, _bpm, _blockSize;
    std::vector<double> _buffer;
    SmoothedValue<float, ValueSmoothingTypes::Linear> _parameterValueSmoothed {0.01f};
    SmoothedValue<float> _feedbackValueSmoothed {0.0f};
};
