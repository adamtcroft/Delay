/*
  ==============================================================================

    SliderWithText.h
    Created: 21 Sep 2020 2:25:22pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "InterfaceDefines.h"
#include "PluginProcessor.h"
#include "BasePanel.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

class SliderWithText : public BasePanel, public Button::Listener
{
public:
    SliderWithText(SuspensionAudioProcessor* processor,
                   String sliderID,
                   String sliderLabel,
                   Rectangle<int> localBounds,
                   bool backgroundGradient = false,
                   String movementID = "");
    
    SliderWithText(SuspensionAudioProcessor* processor,
                   String sliderID,
                   String sliderLabel,
                   Rectangle<int> localBounds,
                   String buttonID,
                   String buttonLabel,
                   bool backgroundGradient = false,
                   String movementID = "");
    
    ~SliderWithText();
    
    void buildSlider(String sliderID);
    void paint(Graphics& g) override;
    void buttonClicked(Button* button) override;
    double yPositionAdjustment();
    void drawAdditionalText(String text, Colour colour = suspensionMaroon, float textSize = 10.f);
    void resized() override;

    std::unique_ptr<Slider> _slider, _movementSpeed;
    bool _drawAdditionalText;
    TextButton _button;
    bool _buttonStateSet;

private:
    String _labelText, _sliderID;
    std::unique_ptr<SliderAttachment> _sliderAttachment, _movementAttachment;
    std::unique_ptr<ButtonAttachment> _buttonAttachment;
    String _additionalText;
    Colour _additionalTextColour;
    float _additionalTextSize;
    bool _gradientFill;
};
