/*
  ==============================================================================

    EffectsPanel.h
    Created: 20 Sep 2020 9:13:27pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BasePanel.h"
#include "SliderWithText.h"
#include "SliderWithTextAndButton.h"
#include "LogoArea.h"

class EffectsPanel : public BasePanel
{
public:
    EffectsPanel(SuspensionAudioProcessor* processor, Rectangle<int> localBounds);
    ~EffectsPanel() = default;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    std::unique_ptr<SliderWithText> _feedbackSlider, _wetDrySlider, _bitReductionSlider, _rateReductionSlider, _delayTimeSlider;
//    std::unique_ptr<SliderWithTextAndButton> _delayTimeSlider;
    std::unique_ptr<LogoArea> _logoArea;
    
    void setDelayTimeButtonState();
};
