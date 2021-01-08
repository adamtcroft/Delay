/*
  ==============================================================================

    MainPanel.h
    Created: 20 Sep 2020 9:13:34pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BasePanel.h"
#include "PresetsPanel.h"
#include "EffectsPanel.h"

class MainPanel : public BasePanel
{
public:
    MainPanel(SuspensionAudioProcessor *processor, Rectangle<int> localBounds);
    ~MainPanel() = default;
    
    void paint(Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<BasePanel> _presetsPanel, _effectsPanel;
};
