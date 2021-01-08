/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "InterfaceDefines.h"
#include "MainPanel.h"
#include "LookAndFeel.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

//==============================================================================
/**
*/
class SuspensionAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SuspensionAudioProcessorEditor (SuspensionAudioProcessor&);
    ~SuspensionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuspensionAudioProcessor& audioProcessor;
    Slider _feedbackSlider, _delayTimeSlider, _wetDrySlider;
    TextButton _freeTimeButton;
    std::shared_ptr<ButtonAttachment> _freeTimeButtonAttachment;
    std::shared_ptr<juce::ButtonParameterAttachment> _buttonListener;
    std::unique_ptr<SliderAttachment> _feedbackAttachment, _delayTimeAttachment, _wetDryAttachment;
    std::unique_ptr<MainPanel> _mainPanel;
    Sus_LookAndFeel* _lookAndFeel;
    int _resizeCount {0};

    //std::unique_ptr<ResizableCornerComponent> _resizableCorner;
    
    //void timerCallback() override;
    bool checkIfVersionOutOfDate();
    void displayUpdatePopup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuspensionAudioProcessorEditor)
};
