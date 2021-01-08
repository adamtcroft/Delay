/*
  ==============================================================================

    SliderWithTextAndButton.h
    Created: 21 Sep 2020 2:25:32pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SliderWithText.h"
#include "InterfaceDefines.h"
#include "PluginProcessor.h"
#include "BasePanel.h"

using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

class SliderWithTextAndButton : public BasePanel, Button::Listener
{
public:
    SliderWithTextAndButton(SuspensionAudioProcessor* processor,
                            String sliderID,
                            String sliderLabel,
                            String buttonID,
                            String buttonLabel,
                            Rectangle<int> localBounds);
    ~SliderWithTextAndButton() = default;
    
    void paint(Graphics& g) override;
    void buttonClicked(Button* button) override;
    void resized() override;
    
    std::unique_ptr<SliderWithText> _swt;
    TextButton _button;
    bool _buttonStateSet;
    
private:
    std::unique_ptr<ButtonAttachment> _buttonAttachment;
};
