/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Gain.h"
#include "Delay.h"
#include "BitCrusher.h"
#include "StateController.h"
#include "Conductor.h"
#include "PresetManager.h"
#include "SignalMover.h"

//==============================================================================
/**
*/
class SuspensionAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SuspensionAudioProcessor();
    ~SuspensionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    StateController _stateController;
    Conductor _conductor;
    std::shared_ptr<PresetManager> _presetManager;
    
    double _delayMinValue {0.1};
    double _delayMaxValue {5.0};
    double _rateReductionMinValue {1.0};
    double _rateReductionMaxValue {50.0};
    
private:
    void initializeDSP();
    
    bool _reverseParam = false;
    float _countParam = 0;
    
    std::vector<std::unique_ptr<Delay>> _delays;
    std::vector<std::unique_ptr<BitCrusher>> _bitcrushers;
    std::unique_ptr<SignalMover> _signalMover[2];
    AudioBuffer<float> _feedbackBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuspensionAudioProcessor)
};
