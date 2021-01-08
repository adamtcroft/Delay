/*
  ==============================================================================

    SliderWithTextAndButton.cpp
    Created: 21 Sep 2020 2:25:32pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "SliderWithTextAndButton.h"

SliderWithTextAndButton::SliderWithTextAndButton(SuspensionAudioProcessor* processor,
                                                 String sliderID,
                                                 String sliderLabel,
                                                 String buttonID,
                                                 String buttonLabel,
                                                 Rectangle<int> localBounds)
:   BasePanel(processor), _buttonStateSet(false)
{
    setBounds(localBounds);
    localBounds = getLocalBounds();
    
    _swt = std::make_unique<SliderWithText>(processor, sliderID, sliderLabel, localBounds, false, "delayMovementID");
    addAndMakeVisible(*_swt);
    
    _button.setButtonText(buttonLabel);
    _button.setBounds(getWidth() * .795, _swt->yPositionAdjustment() + 4, BUTTON_WIDTH, BUTTON_HEIGHT);
    _button.addListener(this);
    addAndMakeVisible(_button);
    
    _buttonAttachment = std::make_unique<ButtonAttachment>(_processor->_stateController._apvts, "freeTimeButtonID", _button);
}

void SliderWithTextAndButton::paint(Graphics& g)
{
    _swt->paint(g);
}

void SliderWithTextAndButton::buttonClicked(Button* button)
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

void SliderWithTextAndButton::resized()
{
    Rectangle<int> bounds = {getWidth(), getHeight()};
    
    if(_swt)
        _swt->setBounds(bounds);
    
    if(_button.isVisible())
        _button.setBounds(getWidth() * .795, _swt->yPositionAdjustment() + 4, BUTTON_WIDTH, BUTTON_HEIGHT);
}
