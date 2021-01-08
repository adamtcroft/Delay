/*
  ==============================================================================

    SliderWithText.cpp
    Created: 21 Sep 2020 2:25:22pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "SliderWithText.h"

SliderWithText::SliderWithText(SuspensionAudioProcessor* processor,
                               String sliderID,
                               String sliderLabel,
                               Rectangle<int> localBounds,
                               bool backgroundGradient,
                               String movementID)
:   BasePanel(processor),
    _drawAdditionalText(false),
    _labelText(sliderLabel),
    _sliderID(sliderID),
    _gradientFill(backgroundGradient)
{
    // SETTING COMPONENT BOUNDS
    setBounds(localBounds);
    localBounds = getLocalBounds();
    
    // INITIALIZE SLIDER
    _slider = std::make_unique<Slider>();
    
    // SET SLIDER STYLE -- (should refactor, maybe?)
    if(sliderID == "delayTimeID" || sliderID == "rateReductionID")
        _slider->setSliderStyle(Slider::SliderStyle::ThreeValueHorizontal);
    else
        _slider->setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    
    // SETTING SLIDER BOUNDS, TEXT STYLE, COLOURS
    _slider->setBounds(SLIDER_X,
                      yPositionAdjustment()+3,
                      (localBounds.getWidth() * .75),
                      SLIDER_HEIGHT);
    _slider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    _slider->setColour(Slider::ColourIds::trackColourId, suspensionMaroon);
    _slider->setColour(Slider::ColourIds::backgroundColourId, suspensionGrey);
    _slider->setColour(Slider::ColourIds::thumbColourId, suspensionGrey);
    
    // MAKE VISIBLE, SETUP ATTACHMENT
    addAndMakeVisible(*_slider);
    _sliderAttachment = std::make_unique<SliderAttachment>(_processor->_stateController._apvts, sliderID, *_slider);

    // CREATE "MOVEMENT" SLIDER
    _movementSpeed = std::make_unique<Slider>();
    _movementSpeed->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _movementSpeed->setBounds(getWidth() * .90, (getHeight()/2) - 11, 50, 40);
    _movementSpeed->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    _movementSpeed->setColour(Slider::ColourIds::rotarySliderFillColourId, suspensionMaroon);
    _movementSpeed->setColour(Slider::ColourIds::rotarySliderOutlineColourId, suspensionGrey);
    _movementSpeed->setColour(Slider::ColourIds::thumbColourId, suspensionGrey);
    
    // CONDITIONAL VISIBILITY ON "MOVEMENT" SLIDER
    if(sliderID == "rateReductionID" ||
       sliderID == "delayTimeID")
        addAndMakeVisible(*_movementSpeed);
    
    // CONDITIONAL ATTACHMENT INSTANTIATION ON "MOVEMENT" SLIDER
    if(movementID != "")
        _movementAttachment = std::make_unique<SliderAttachment>(_processor->_stateController._apvts, movementID, *_movementSpeed);
}

SliderWithText::SliderWithText(SuspensionAudioProcessor* processor,
                               String sliderID,
                               String sliderLabel,
                               Rectangle<int> localBounds,
                               String buttonID,
                               String buttonLabel,
                               bool backgroundGradient,
                               String movementID)
    :   SliderWithText(processor, sliderID, sliderLabel, localBounds, backgroundGradient, movementID)
{
    _button.setButtonText(buttonLabel);
    _button.setBounds(getWidth() * .795, yPositionAdjustment() + 4, BUTTON_WIDTH, BUTTON_HEIGHT);
    _button.addListener(this);
    addAndMakeVisible(_button);
    
    _buttonAttachment = std::make_unique<ButtonAttachment>(_processor->_stateController._apvts, "freeTimeButtonID", _button);
}


SliderWithText::~SliderWithText()
{
    _sliderAttachment.reset();
    _movementAttachment.reset();
}

void SliderWithText::paint(Graphics& g)
{
    g.setFont(Font(oswaldRegular).withPointHeight(SLIDER_LABEL_HEIGHT));
    
    if(_gradientFill)
    {
        Rectangle<int> gradientRectangle
        {
            getLocalBounds().getX(),
            getLocalBounds().getY(),
            getLocalBounds().getWidth(),
            getLocalBounds().getHeight()
        };
        
        ColourGradient gradient
        {
            Colours::black,
            static_cast<float>(getLocalBounds().getWidth()/2),
            static_cast<float>(getLocalBounds().getY()-20),
            Colours::white,
            static_cast<float>(getLocalBounds().getWidth()/2),
            static_cast<float>(getLocalBounds().getY()+8),
            false
        };
        g.setGradientFill(gradient);
        g.fillRect(gradientRectangle);
    }
    else
    {
        g.fillAll(Colours::white);
    }

    g.setColour(Colours::black);
    g.drawFittedText(_labelText,
                     SLIDER_X,
                     yPositionAdjustment() - SLIDER_LABEL_HEIGHT,
                     150,
                     SLIDER_LABEL_HEIGHT,
                     Justification::left,
                     1);
    
    if(_drawAdditionalText)
    {
        g.setColour(_additionalTextColour);
        g.setFont(Font(oswaldRegular).withPointHeight(_additionalTextSize));
        g.drawFittedText(_additionalText,
                         0,
                         yPositionAdjustment() - SLIDER_LABEL_HEIGHT,
                         _slider->getWidth(),
                         SLIDER_LABEL_HEIGHT,
                         Justification::right,
                         1);
    }
    
    if(_movementSpeed->getValue() > 0.0)
    {
        auto rateValue = _processor->_stateController._apvts.getParameter(_sliderID)->getValue();
        rateValue = jmap(rateValue,
                         _processor->_stateController._apvts.getParameter(_sliderID)->getNormalisableRange().start,
                         _processor->_stateController._apvts.getParameter(_sliderID)->getNormalisableRange().end);
        _slider->setValue(rateValue);
        
        if(_sliderID == "delayTimeID")
        {
            if(_slider->getMinValue() < 0.01)
                _processor->_delayMinValue = 0.01;
            else
                _processor->_delayMinValue = _slider->getMinValue();
            
            _processor->_delayMaxValue = _slider->getMaxValue();
        }
        
        if(_sliderID == "rateReductionID")
        {
            if(_slider->getMinValue() < 1)
                _processor->_rateReductionMinValue = 1;
            else
                _processor->_rateReductionMinValue = _slider->getMinValue();
            
            _processor->_rateReductionMaxValue = _slider->getMaxValue();
        }
        
        repaint();
    }
}

void SliderWithText::buttonClicked(Button* button)
{
    if(button->getToggleState() == false)
    {
        button->setToggleState(true, NotificationType::dontSendNotification);
    }
    else
        button->setToggleState(false, NotificationType::dontSendNotification);
    
    _buttonStateSet = false;
    repaint();
}

double SliderWithText::yPositionAdjustment()
{
    return (getLocalBounds().getHeight()/2) - (SLIDER_HEIGHT/4);

}

void SliderWithText::drawAdditionalText(String text, Colour colour, float textSize)
{
    _drawAdditionalText = true;
    _additionalText = text;
    _additionalTextColour = colour;
    _additionalTextSize = textSize;
}

void SliderWithText::resized()
{
    if(_slider)
    {
        _slider->setBounds(SLIDER_X,
                           yPositionAdjustment()+3,
                           (getWidth() * .75),
                           SLIDER_HEIGHT);
    }
    
    if(_movementSpeed)
    {
        _movementSpeed->setBounds(getWidth() * .90, (getHeight()/2) - 11, 50, 40);
    }
    
    if(_button.isVisible())
        _button.setBounds(getWidth() * .795, yPositionAdjustment() + 4, BUTTON_WIDTH, BUTTON_HEIGHT);
}
