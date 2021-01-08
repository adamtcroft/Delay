/*
  ==============================================================================

    LogoArea.h
    Created: 13 Oct 2020 8:13:50pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "BasePanel.h"
#include "InterfaceDefines.h"

class Logo : public Component
{
public:
    Logo(Rectangle<int> parent)
    {
        Rectangle<int> bounds {(parent.getWidth()/2)-75, (parent.getHeight()/2)-25, 150, 60};
        setBounds(bounds);
    }
    ~Logo() = default;
    
    void paint(Graphics& g) override
    {
        g.setColour(Colours::black);
        g.setFont(Font(oswaldLight).withPointHeight(30));
        g.drawText("SUSP   NSION", 0, 0, getWidth(), getHeight()-30, Justification::centred);
        
        /* -- LOGO LINES -- */
        g.setColour(Colours::black);
        g.drawLine(62, 4, 78, 4, 1.5);
        g.drawLine(62, 15, 78, 15, 1.5);
        
        g.setColour(Colours::grey);
        g.drawLine(62, 27, 78, 27, 1.5); //+15
        
        g.setColour(Colour(146, 146, 146));
        g.drawLine(62, 38, 78, 38, 1.5);
        g.setColour(Colour(213, 213, 213));
        g.drawLine(62, 49, 78, 49, 1.5);
        /* -- END LOGO LINES -- */
    }
    
    void resized() override
    {
        if(getParentWidth() < 1920 && getParentHeight() < 1080)
        {
            
        }
    }
};

class LogoArea : public BasePanel, public Button::Listener
{
public:
    LogoArea(SuspensionAudioProcessor* processor, Rectangle<int> localBounds);
    ~LogoArea() = default;
    
    void paint(Graphics& g) override;
    void buttonClicked(Button* button) override;
    void resized() override;
    
    TextButton _submitBugButton;
    TextButton _userFeedbackButton;
    std::unique_ptr<Logo> _logo;
};
