/*
  ==============================================================================

    BasePanel.h
    Created: 20 Sep 2020 9:19:48pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"

class BasePanel : public Component
{
public:
    BasePanel(SuspensionAudioProcessor* processor);
    ~BasePanel() = default;
    
    void paint(Graphics& g) override;
    
protected:
    SuspensionAudioProcessor* _processor;
};
