/*
  ==============================================================================

    PresetsPanel.h
    Created: 20 Sep 2020 9:13:07pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BasePanel.h"
#include "InterfaceDefines.h"
#include "LookAndFeel.h"

class PresetsPanel : public BasePanel, public Button::Listener, public ComboBox::Listener
{
public:
    PresetsPanel(SuspensionAudioProcessor* processor, Rectangle<int> localBounds);
    ~PresetsPanel();
    
    void paint(Graphics& g) override;
    void buttonClicked(Button* button) override;
    void comboBoxChanged(ComboBox* comboBox) override;
    void resized() override;
    
private:
    std::unique_ptr<ComboBox> _presetPicker;
    std::unique_ptr<TextButton> _newPreset, _savePreset, _saveAsPreset, _removePreset, _loadPreset;
    
    void updatePresetComboBox();
    void displaySaveAsPopup();
    void displayLoadPopup();
};
