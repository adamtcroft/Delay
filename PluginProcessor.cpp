/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuspensionAudioProcessor::SuspensionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        _stateController(*this, nullptr, "PARAMETERS")
#endif
{
    _presetManager = std::make_shared<PresetManager>(this);
    initializeDSP();
}

SuspensionAudioProcessor::~SuspensionAudioProcessor()
{
}

//==============================================================================
const String SuspensionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SuspensionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SuspensionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SuspensionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SuspensionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SuspensionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SuspensionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SuspensionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SuspensionAudioProcessor::getProgramName (int index)
{
    return {};
}

void SuspensionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SuspensionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        _delays[i]->setSampleRate(sampleRate);
    }
}

void SuspensionAudioProcessor::releaseResources()
{
    for (int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        _delays[i]->reset(getSampleRate());
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuspensionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SuspensionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (juce::AudioPlayHead* playhead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo positionInfo;
        playhead->getCurrentPosition(positionInfo);
        _conductor.update(positionInfo, getSampleRate());
    }
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
            
    _signalMover[0]->process(_stateController._apvts.getParameter("rateReductionID"),
                             _stateController._apvts.getParameter("rateMovementID")->getValue(),
                             _rateReductionMinValue,
                             _rateReductionMaxValue);
    
    _signalMover[1]->process(_stateController._apvts.getParameter("delayTimeID"),
                             _stateController._apvts.getParameter("delayMovementID")->getValue(),
                             _delayMinValue,
                             _delayMaxValue);
    
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    AudioBuffer<float> effectsBuffer;
    effectsBuffer.makeCopyOf(buffer);
    
    if(_feedbackBuffer.getNumSamples() == 0)
    {
        _feedbackBuffer.makeCopyOf(effectsBuffer);
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* processedData = effectsBuffer.getWritePointer(channel);
        auto* feedbackData = _feedbackBuffer.getWritePointer(channel);
        auto* output = buffer.getWritePointer(channel);
        
        float feedbackValue = jmap(_stateController._apvts.getParameter("feedbackID")->getValue(), 0.f, 1.f, 0.f, 0.95f);
        
        insertFeedback(processedData, feedbackData, feedbackValue, buffer.getNumSamples());
        
        _delays[channel]->setBPM(_conductor._bpm);
        _delays[channel]->process(processedData,
                                 _stateController._apvts.getParameter("delayTimeID")->getValue(),
                                 _stateController._apvts.getParameter("wetDryID")->getValue(),
                                 _stateController._apvts.getParameter("freeTimeButtonID")->getValue(),
                                 processedData,
                                 effectsBuffer.getNumSamples());
        
        //copyChannel(feedbackData, effectsData, buffer.getNumSamples());
        
        _feedbackBuffer.copyFrom(channel, 0, effectsBuffer, channel, 0, buffer.getNumSamples());

        _bitcrushers[channel]->process(processedData,
                                      effectsBuffer.getNumSamples(),
                                      _stateController._apvts.getParameter("rateReductionID")->getValue(),
                                      _stateController._apvts.getParameter("bitReductionID")->getValue());

        wetDryMix(output,
                  processedData,
                  _stateController._apvts.getParameter("wetDryID")->getValue(),
                  effectsBuffer.getNumSamples());
    }
}

//==============================================================================
bool SuspensionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SuspensionAudioProcessor::createEditor()
{
    return new SuspensionAudioProcessorEditor (*this);
}

//==============================================================================
void SuspensionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    XmlElement preset("StateInfo");
    XmlElement* presetBody = new XmlElement("Preset");
    _presetManager->getXmlForPreset(presetBody);
    preset.addChildElement(presetBody);
    copyXmlToBinary(preset, destData);
}

void SuspensionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<XmlElement> XmlState = getXmlFromBinary(data, sizeInBytes);
    
    if(XmlState)
    {
        forEachXmlChildElement(*XmlState, subChild)
        {
            _presetManager->loadPresetForXml(subChild);
        }
    }
    else
        jassertfalse;
}

void SuspensionAudioProcessor::initializeDSP()
{
    for (int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        _delays.push_back(std::make_unique<Delay>());
        _bitcrushers.push_back(std::make_unique<BitCrusher>());
    }
    
    _signalMover[0] = std::make_unique<SignalMover>();
    _signalMover[1] = std::make_unique<SignalMover>();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuspensionAudioProcessor();
}
