/*
  ==============================================================================

    LookAndFeel.h
    Created: 3 Oct 2020 10:04:43pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "InterfaceDefines.h"

class Sus_LookAndFeel : public juce::LookAndFeel_V4
{
public:
    Sus_LookAndFeel()
    {
        /* TextButton Color settings*/
        setColour(TextButton::buttonColourId, Colours::white);
        setColour(TextButton::buttonOnColourId, suspensionMaroon);
        setColour(TextButton::textColourOnId, Colours::white);
        setColour(TextButton::textColourOffId, Colours::black);
        
        /* ComboBox Color settings */
        setColour(ComboBox::backgroundColourId, Colours::white);
        setColour(ComboBox::textColourId, Colours::black);
        setColour(ComboBox::arrowColourId, Colours::black);
        setColour(PopupMenu::backgroundColourId, Colours::white);
        setColour(PopupMenu::textColourId, Colours::black);
        setColour(PopupMenu::highlightedBackgroundColourId, Colours::maroon);
        
        setColour(AlertWindow::backgroundColourId, Colours::white);
        setColour(AlertWindow::textColourId, Colours::black);
        setColour(TextEditor::backgroundColourId, Colours::white);
        setColour(TextEditor::textColourId, Colours::black);
        setColour(TextEditor::highlightColourId, Colours::maroon);
    };
    ~Sus_LookAndFeel() = default;
    
    Font getTextButtonFont(TextButton& b, int buttonHeight) override
    {
        if(b.getName() == "Confirm"|| b.getName() == "Cancel")
            return Font(oswaldRegular).withPointHeight(ALERT_WINDOW_BUTTON_FONT_HEIGHT);
        //else if(b.getName() == "userFeedbackButton" || b.getName() == "submitBugButton")
          //  return Font(oswaldRegular).withPointHeight(USER_FEEDBACK_BUTTON_FONT_HEIGHT);
        else
            return Font(oswaldRegular).withPointHeight(BUTTON_FONT_HEIGHT);
    }
    
    Font getComboBoxFont (juce::ComboBox&) override
    {
        return Font(oswaldRegular).withPointHeight(COMBO_BOX_FONT_HEIGHT);
    }
    
    Font getPopupMenuFont() override
    {
        return Font(oswaldRegular).withPointHeight(POPUP_MENU_FONT_HEIGHT);
    }
    
    Font getAlertWindowTitleFont() override
    {
        return Font(oswaldRegular).withPointHeight(ALERT_WINDOW_TITLE_FONT_HEIGHT);
    }
    
    Font getAlertWindowMessageFont() override
    {
        return Font(oswaldRegular).withPointHeight(ALERT_WINDOW_MESSAGE_FONT_HEIGHT);
    }
    
    Font getAlertWindowFont() override
    {
        return getAlertWindowMessageFont();
    }
    
    virtual void getIdealPopupMenuItemSize(const juce::String & text,
                                           bool isSeparator,
                                           int standardMenuItemHeight,
                                           int& idealWidth,
                                           int& idealHeight) override
    {
        idealHeight = 21;
        standardMenuItemHeight = idealHeight;
    }
    
    int getAlertWindowButtonHeight() override
    {
        return ALERT_WINDOW_BUTTON_HEIGHT;
    }
};
