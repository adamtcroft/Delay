/*
  ==============================================================================

    MainPanel.cpp
    Created: 20 Sep 2020 9:13:34pm
    Author:  Adam T. CroftÏ

  ==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel(SuspensionAudioProcessor* processor, Rectangle<int> localBounds)
:   BasePanel(processor)
{
    setBounds(localBounds);
    _presetsPanel = std::make_unique<PresetsPanel>(processor, localBounds.removeFromTop(REMOVE_FOR_PRESETS_PANEL));
    _effectsPanel = std::make_unique<EffectsPanel>(processor, localBounds);
    
    addAndMakeVisible(*_presetsPanel);
    addAndMakeVisible(*_effectsPanel);
}

void MainPanel::paint(Graphics &g)
{
}

void MainPanel::resized()
{
    Rectangle<int> bounds {getParentWidth(), getParentHeight()};
    
    if(bounds.getWidth() < 1920 && bounds.getHeight() < 1080)
    {
        setBounds(bounds);
    }
    
    if(_presetsPanel)
        _presetsPanel->setBounds(bounds.removeFromTop(REMOVE_FOR_PRESETS_PANEL));
    
    if(_effectsPanel)
        _effectsPanel->setBounds(bounds);
}
