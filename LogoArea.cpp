/*
  ==============================================================================

    LogoArea.cpp
    Created: 13 Oct 2020 8:13:50pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "LogoArea.h"

LogoArea::LogoArea(SuspensionAudioProcessor* processor, Rectangle<int> localBounds)
:   BasePanel(processor)
{
    setBounds(localBounds);
    localBounds = getLocalBounds();
    
    _userFeedbackButton.setButtonText("SUBMIT FEEDBACK");
    _userFeedbackButton.setName("userFeedbackButton");
    _userFeedbackButton.setBounds(3, localBounds.getHeight() - ((BUTTON_HEIGHT*2)+20), FEEDBACK_BUTTONS_WIDTH, FEEDBACK_BUTTONS_HEIGHT);
    _userFeedbackButton.addListener(this);
    addAndMakeVisible(_userFeedbackButton);
    
    _submitBugButton.setButtonText("SUBMIT BUG");
    _submitBugButton.setName("submitBugButton");
    _submitBugButton.setBounds(3, localBounds.getHeight() - (BUTTON_HEIGHT+10), FEEDBACK_BUTTONS_WIDTH, FEEDBACK_BUTTONS_HEIGHT);
    _submitBugButton.addListener(this);
    addAndMakeVisible(_submitBugButton);
    
    _logo = std::make_unique<Logo>(getLocalBounds());
    addAndMakeVisible(*_logo);
}

void LogoArea::paint(Graphics& g)
{
    g.fillAll(Colours::white);
 
    g.setFont(Font(oswaldLight).withPointHeight(9));
    g.drawText("v" + std::to_string(VERSION_NUMBER).substr(0,3), getWidth()-40, getHeight()-10, 20, 10, Justification::bottomRight);
}

void LogoArea::buttonClicked(Button* button)
{
    if(button == &_submitBugButton)
        URL("https://forms.gle/xF9EDHMFiGcyXdTj6").launchInDefaultBrowser();
    
    if(button == &_userFeedbackButton)
        URL("https://forms.gle/EJffAs588VcbEcbE6").launchInDefaultBrowser();
}

void LogoArea::resized()
{
    Rectangle<int> bounds {(getWidth()/2)-75, (getHeight()/2)-25, 150, 60};

    if(_logo)
        _logo->setBounds(bounds);
    
    if(_userFeedbackButton.isVisible())
        _userFeedbackButton.setBounds(3, getHeight() - ((BUTTON_HEIGHT*2)+20), FEEDBACK_BUTTONS_WIDTH, FEEDBACK_BUTTONS_HEIGHT);
    
    if(_submitBugButton.isVisible())
        _submitBugButton.setBounds(3, getHeight() - (BUTTON_HEIGHT+10), FEEDBACK_BUTTONS_WIDTH, FEEDBACK_BUTTONS_HEIGHT);
}
