/*
  ==============================================================================

    EffectsPanel.cpp
    Created: 20 Sep 2020 9:13:27pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "EffectsPanel.h"

EffectsPanel::EffectsPanel(SuspensionAudioProcessor* processor, Rectangle<int> localBounds)
:   BasePanel(processor)
{
    setBounds(localBounds);
    localBounds = getLocalBounds();
        
    auto sliderBounds = localBounds.getHeight()/NUMBER_OF_SLIDERS;
    
    _feedbackSlider = std::make_unique<SliderWithText>(_processor,
                                                       "feedbackID",
                                                       "Feedback",
                                                       localBounds.removeFromTop(sliderBounds),
                                                       true);
    
    // YO DUMMY YOU'VE HARD CODED THE DELAY MOVEMENT SLIDER INSIDE THE SLIDER WITH TEXT AND BUTTON CLASS BECAUSE
    // YOU'RE ONLY USING THIS ONCE - WHY DID YOU MAKE IT A SEPARATE CLASS????
    _delayTimeSlider = std::make_unique<SliderWithText>(_processor,
                                                        "delayTimeID",
                                                        "Delay Time",
                                                        localBounds.removeFromTop(sliderBounds),
                                                        "freeTimeButtonID",
                                                        "FREE",
                                                        false,
                                                        "delayMovementID");
    
    _delayTimeSlider->_slider->setRange(Range<double>(0.01, 5.00), 1.00);
    auto delayMinParameter = _processor->_stateController._apvts.getParameter("delayMinID");
    auto delayMaxParameter = _processor->_stateController._apvts.getParameter("delayMaxID");
    auto delayMinValue = delayMinParameter->convertFrom0to1(delayMinParameter->getValue());
    auto delayMaxValue = delayMaxParameter->convertFrom0to1(delayMaxParameter->getValue());
    _delayTimeSlider->_slider->setMinValue(delayMinValue);
    _delayTimeSlider->_slider->setMaxValue(delayMaxValue);
        
    _bitReductionSlider = std::make_unique<SliderWithText>(_processor,
                                                       "bitReductionID",
                                                       "Bit Reduction",
                                                       localBounds.removeFromTop(sliderBounds));
    _bitReductionSlider->_slider->setRange(2.0, 24.0);
    _bitReductionSlider->_slider->setSkewFactor(1.9, true);
    _bitReductionSlider->_slider->setSkewFactorFromMidPoint(6);
    
    _rateReductionSlider = std::make_unique<SliderWithText>(_processor,
                                                            "rateReductionID",
                                                            "Sample Rate Reduction",
                                                            localBounds.removeFromTop(sliderBounds),
                                                            false,
                                                            "rateMovementID");
    _rateReductionSlider->_slider->setRange(1, 50);
    auto rateMinParameter = _processor->_stateController._apvts.getParameter("rateMinID");
    auto rateMaxParameter = _processor->_stateController._apvts.getParameter("rateMaxID");
    auto rateMinValue = rateMinParameter->convertFrom0to1(rateMinParameter->getValue());
    auto rateMaxValue = rateMaxParameter->convertFrom0to1(rateMaxParameter->getValue());
    _rateReductionSlider->_slider->setMinValue(rateMinValue);
    _rateReductionSlider->_slider->setMaxValue(rateMaxValue);
    
    _wetDrySlider = std::make_unique<SliderWithText>(_processor,
                                                     "wetDryID",
                                                     "Dry",
                                                     localBounds.removeFromTop(sliderBounds));
    
    _logoArea = std::make_unique<LogoArea>(_processor, localBounds.removeFromTop(sliderBounds));
    
    addAndMakeVisible(*_feedbackSlider);
    addAndMakeVisible(*_delayTimeSlider);
    addAndMakeVisible(*_bitReductionSlider);
    addAndMakeVisible(*_rateReductionSlider);
    addAndMakeVisible(*_wetDrySlider);
    addAndMakeVisible(*_logoArea);
}

void EffectsPanel::paint(Graphics& g)
{    
    g.fillAll(Colours::white);
    setDelayTimeButtonState();
    _wetDrySlider->drawAdditionalText("Wet", Colours::black, 12.f);
}

void EffectsPanel::resized()
{
    Rectangle<int> bounds {getWidth(), getHeight()};
    
    auto height = bounds.getHeight()/NUMBER_OF_SLIDERS;
    
    if(_feedbackSlider)
        _feedbackSlider->setBounds(bounds.removeFromTop(height));
    
    if(_delayTimeSlider)
    {
        _delayTimeSlider->setBounds(bounds.removeFromTop(height));
        setDelayTimeButtonState();
        _delayTimeSlider->repaint();
    }
    
    if(_bitReductionSlider)
        _bitReductionSlider->setBounds(bounds.removeFromTop(height));
    
    if(_rateReductionSlider)
        _rateReductionSlider->setBounds(bounds.removeFromTop(height));
    
    if(_wetDrySlider)
        _wetDrySlider->setBounds(bounds.removeFromTop(height));
    
    if(_logoArea)
        _logoArea->setBounds(bounds.removeFromTop(height));
}

void EffectsPanel::setDelayTimeButtonState()
{
    if(_delayTimeSlider->_button.getToggleState() == false)
    {
        auto timeSwitch = static_cast<int>(_processor->_stateController._apvts.getParameter("delayTimeID")->getValue()*10);
        String delayTimeString;

        switch(timeSwitch)
        {
            case 0:
                delayTimeString = "1/32";
                break;
            case 2:
                delayTimeString = "1/16";
                break;
            case 4:
                delayTimeString = "1/8";
                break;
            case 6:
                delayTimeString = "1/4";
                break;
            case 8:
                delayTimeString = "1/2";
                break;
            case 10:
                delayTimeString = "Whole";
                break;
            default:
                delayTimeString = "";
                break;
        }
        repaint();
        
        _delayTimeSlider->drawAdditionalText(delayTimeString);
    }
    
    if(_delayTimeSlider->_button.getToggleState() == true &&
       _delayTimeSlider->_buttonStateSet == false)
    {
        _delayTimeSlider->_drawAdditionalText = false;
        _delayTimeSlider->_slider->setRange(0.01, 5);
        _delayTimeSlider->_slider->setSkewFactorFromMidPoint(1.8);
        _delayTimeSlider->_buttonStateSet = true;
    }
    else if(_delayTimeSlider->_button.getToggleState() == false &&
            _delayTimeSlider->_buttonStateSet == false)
    {
        _delayTimeSlider->_slider->setRange(Range<double>(0.01, 5.00), 1.00);
        _delayTimeSlider->_slider->setSkewFactorFromMidPoint(3.0);
        _delayTimeSlider->_buttonStateSet = true;
    }
}
