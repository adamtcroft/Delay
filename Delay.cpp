/*
  ==============================================================================

    Delay.cpp
    Created: 24 Aug 2020 9:52:27pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay()
:   _feedbackSample(0.0),
    _timeSmoothed(0.0f),
    _feedbackSmoothed(0.0f),
    _feedbackTargetValue(0.0f),
    _delayTimeTargetValue(0.0f),
    _sampleRate(-1),
    _delayIndex(0),
    _bpm(-1),
    _blockSize(0)
{
}

void Delay::setSampleRate(double sampleRate)
{
    _sampleRate = sampleRate;
    reset(sampleRate);
    _parameterValueSmoothed.reset(_sampleRate, parameterSmoothingCoefficient);
    _feedbackValueSmoothed.reset(_sampleRate, feedbackSmoothingCoefficient);
}

void Delay::reset(double sampleRate)
{
    _buffer.clear();
    _buffer.resize(sampleRate * 4);
    _buffer.shrink_to_fit();
}

void Delay::process(float* inputAudio,
                    float delayTime,
                    float wetDry,
                    int delayState,
                    float* outputAudio,
                    int numberOfSamplesToRender)
{

        
    for(int i = 0; i < numberOfSamplesToRender; i++)
    {
        // takes a time in seconds and turns it into time in samples
        double delayTimeInSamples;
        if(delayState == 0)
            delayTimeInSamples = getDelayFreeTime(delayTime);
        else
             delayTimeInSamples = getDelaySyncedTime(delayTime);
        
        // Sends the delay time in samples to getInterpolatedSample
        // Which returns an interpolated sample from the delay buffer
        const double interpolatedSample = getInterpolatedSample(delayTimeInSamples);
        
        // adds the feedback to the inputted audio, creating a feedback loop, and taking into
        // account the user-desired feedback amount
        _buffer[_delayIndex] = inputAudio[i];
        
        outputAudio[i] = interpolatedSample;

        _delayIndex = _delayIndex + 1;
        
        if (_delayIndex >= _buffer.size())
        {
            _delayIndex = _delayIndex - (static_cast<int>(_buffer.size()));
        }
    }
}

double Delay::getInterpolatedSample(float delayTimeInSamples)
{
    // Defines the current read position as the read index minus the delay time in samples
    double readPosition = static_cast<double>(_delayIndex) - delayTimeInSamples;
    
    // if the read position is negative...
    if (readPosition < 0.0)
    {
        // Then the read position is actually whatever that negative number is
        // plus the total number of indices in the array
        readPosition = readPosition + (_buffer.size());
    }
    
    // index_y0 is the read position, but back one sample
    int index_y0 = static_cast<int>(readPosition) + 1;
    
    // catch and loop index_y0 back to the beginning
    if(index_y0 > _buffer.size()-1)
        index_y0 = index_y0 - (static_cast<int>(_buffer.size()));
        
    // if that position is less than or equal to zero...
    if (index_y0 < 0)
    {
        // Then this index is that negative number plus the total number of
        // indices in the array
        index_y0 = index_y0 + (static_cast<int>(_buffer.size()));
    }
    
    // index of sample y1 is just the read position
    int index_y1 = static_cast<int>(readPosition);
    // if that index is bigger than the total size of the buffer...
    if (index_y1 > _buffer.size())
    {
        // then the index wraps around and is actually that number minus the
        // size of the whole buffer
        index_y1 = index_y1 - (static_cast<int>(_buffer.size()));
    }
    
    // now we want to get our delayed samples to run through linear interp...
    if(isnan(_buffer[index_y0]))
        _buffer[index_y0] = 0;
        
    const float sample_y0 = _buffer[index_y0];
    
    if(isnan(_buffer[index_y1]))
        _buffer[index_y1] = 0;

    const float sample_y1 = _buffer[index_y1];

    const float t = readPosition - static_cast<int>(readPosition);
    
    double outputSample = linearInterp(sample_y0, sample_y1, t);
    
    return outputSample;
}

int Delay::getDelaySyncedTime(float delayTime)
{
    float calculatedTime;
        
    if(_bpm > 0)
    {
        calculatedTime = (60.0/_bpm) * _sampleRate;
        
        switch(static_cast<int>(10*delayTime))
        {
            case 0:
                calculatedTime = calculatedTime/8;
                break;
            case 2:
                calculatedTime = calculatedTime/4;
                break;
            case 4:
                calculatedTime = calculatedTime/2;
                break;
            case 6:
                break;
            case 8:
                calculatedTime = calculatedTime * 2;
                break;
            case 10:
                calculatedTime = calculatedTime * 4;
                break;
            default:
                break;
        }
    }
    else
    {
        calculatedTime = 0;
    }
    return static_cast<int>(calculatedTime);
}

int Delay::getDelayFreeTime(double delayTime)
{
    if(delayTime < 0.0)
        delayTime = 0.0;
    if(delayTime > 1.0)
        delayTime = 1.0;
    
    delayTime = jmap(delayTime, 0.01, 1.0, 0.01, 2.0);
    if(_delayTimeTargetValue != delayTime)
    {
        _delayTimeTargetValue = delayTime;
        _parameterValueSmoothed.setTargetValue(_delayTimeTargetValue);
    }
    
    _timeSmoothed = _parameterValueSmoothed.getNextValue();
    return static_cast<int>(_timeSmoothed * _sampleRate);
}

void Delay::setBPM(int bpm)
{
    _bpm = bpm;
}
