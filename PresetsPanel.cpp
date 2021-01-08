/*
  ==============================================================================

    PresetsPanel.cpp
    Created: 20 Sep 2020 9:13:07pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "PresetsPanel.h"

PresetsPanel::PresetsPanel(SuspensionAudioProcessor* processor, Rectangle<int> localBounds)
:   BasePanel(processor)
{
    setBounds(localBounds);
    
    int componentY = (getLocalBounds().getHeight()/2) - (BUTTON_HEIGHT/2);
    float space = (getLocalBounds().getWidth() - (BUTTON_WIDTH * 5) - COMBO_BOX_WIDTH)/7;
    
    _loadPreset = std::make_unique<TextButton>();
    _loadPreset->setBounds(space, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    _loadPreset->setButtonText("LOAD");
    _loadPreset->addListener(this);
    
    _newPreset = std::make_unique<TextButton>();
    _newPreset->setBounds((space * 2) + BUTTON_WIDTH, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    _newPreset->setButtonText("NEW");
    _newPreset->addListener(this);
    
    _savePreset = std::make_unique<TextButton>();
    _savePreset->setBounds((space * 3) + (BUTTON_WIDTH * 2), componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    _savePreset->setButtonText("SAVE");
    _savePreset->addListener(this);
    
    _saveAsPreset = std::make_unique<TextButton>();
    _saveAsPreset->setBounds((space * 4) + (BUTTON_WIDTH * 3), componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    _saveAsPreset->setButtonText("SAVE AS");
    _saveAsPreset->addListener(this);
    
    _presetPicker = std::make_unique<ComboBox>();
    _presetPicker->setBounds((space * 5) + (BUTTON_WIDTH * 4), componentY, COMBO_BOX_WIDTH, BUTTON_HEIGHT);
    _presetPicker->setText("Presets");
    _presetPicker->addListener(this);
    
    _removePreset = std::make_unique<TextButton>();
    _removePreset->setBounds((space * 6) + (BUTTON_WIDTH * 4) + COMBO_BOX_WIDTH, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    _removePreset->setButtonText("REMOVE");
    _removePreset->addListener(this);
    
    addAndMakeVisible(*_loadPreset);
    addAndMakeVisible(*_newPreset);
    addAndMakeVisible(*_savePreset);
    addAndMakeVisible(*_saveAsPreset);
    addAndMakeVisible(*_presetPicker);
    addAndMakeVisible(*_removePreset);
    
    _processor->_presetManager->loadStateOnOpen();
    updatePresetComboBox();
}

PresetsPanel::~PresetsPanel()
{
}

void PresetsPanel::paint(Graphics& g)
{
    g.fillAll(suspensionGrey);
}

void PresetsPanel::buttonClicked(Button* button)
{
    if(button == _newPreset.get())
    {
        _processor->_presetManager->createNewPreset();
        updatePresetComboBox();
    }
    else if(button == _savePreset.get())
    {
        if(_processor->_presetManager->getNumberofPresets() == 0 ||
           !_processor->_presetManager->_currentlyLoadedPreset.existsAsFile())
            displaySaveAsPopup();
        else
            _processor->_presetManager->savePreset();
    }
    else if(button == _saveAsPreset.get())
        displaySaveAsPopup();
    else if(button == _removePreset.get())
    {
        _processor->_presetManager->removePreset();
        updatePresetComboBox();
    }
    else if (button == _loadPreset.get())
        displayLoadPopup();
}

void PresetsPanel::comboBoxChanged(ComboBox* comboBox)
{
    if(comboBox == _presetPicker.get())
    {
        const int index = _presetPicker->getSelectedItemIndex();
        _processor->_presetManager->loadPreset(index);
    }
}

void PresetsPanel::resized()
{
    int componentY = (getHeight()/2) - (BUTTON_HEIGHT/2);
    float space = (getWidth() - (BUTTON_WIDTH * 5) - COMBO_BOX_WIDTH)/7;
    
    if(_loadPreset)
        _loadPreset->setBounds(space, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);

    if(_newPreset)
        _newPreset->setBounds((space * 2) + BUTTON_WIDTH, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);

    if(_savePreset)
        _savePreset->setBounds((space * 3) + (BUTTON_WIDTH * 2), componentY, BUTTON_WIDTH, BUTTON_HEIGHT);

    if(_saveAsPreset)
        _saveAsPreset->setBounds((space * 4) + (BUTTON_WIDTH * 3), componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
    
    if(_presetPicker)
        _presetPicker->setBounds((space * 5) + (BUTTON_WIDTH * 4), componentY, COMBO_BOX_WIDTH, BUTTON_HEIGHT);

    if(_removePreset)
        _removePreset->setBounds((space * 6) + (BUTTON_WIDTH * 4) + COMBO_BOX_WIDTH, componentY, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void PresetsPanel::updatePresetComboBox()
{
    String currentPresetName = _processor->_presetManager->getCurrentPresetName();
    
    _presetPicker->clear(dontSendNotification);
    
    const int numberOfPresets = _processor->_presetManager->getNumberofPresets();
    
    for(int i = 0; i < numberOfPresets; i++)
        _presetPicker->addItem(_processor->_presetManager->getPresetName(i), i+1);
    
    _presetPicker->setText(currentPresetName);
}

void PresetsPanel::displaySaveAsPopup()
{
    String currentPresetName = _processor->_presetManager->getCurrentPresetName();
    
    AlertWindow alertWindow ("Save As...", "Please choose a name and location for your preset", AlertWindow::NoIcon);
    alertWindow.setLookAndFeel(&getLookAndFeel());
    
    File presetDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator + "NIR Plugin Presets" + directorySeparator +
    _processor->getName() + "Presets";
    
    std::unique_ptr<FilenameComponent> fnc = std::make_unique<FilenameComponent>("", presetDirectory, false, true, true, "*.nir", "", "Untitled");

    alertWindow.centreAroundComponent(this, getWidth(), getHeight());
    fnc->setBounds(0, 0, getWidth(), getHeight());
    alertWindow.addCustomComponent(fnc.get());
    alertWindow.addTextEditor("Filename", "Untitled", "Filename:");
    alertWindow.addButton("Confirm", 1, KeyPress(KeyPress::returnKey));
    alertWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));
    

    if(alertWindow.runModalLoop())
    {
        _processor->_presetManager->saveAsPreset(alertWindow.getTextEditorContents("Filename"), fnc->getCurrentFileText());
        updatePresetComboBox();
    }
}

void PresetsPanel::displayLoadPopup()
{
    AlertWindow alertWindow ("Load...", "Please choose your preset", AlertWindow::NoIcon);
    alertWindow.setLookAndFeel(&getLookAndFeel());
    
    File preset = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator +
    _processor->getName() + "Presets";
    std::unique_ptr<FilenameComponent> fnc = std::make_unique<FilenameComponent>("", preset, false, false, false, "*.nir", "", "Untitled");
    
    alertWindow.centreAroundComponent(this, getWidth(), getHeight());
    fnc->setBounds(0, 0, getWidth(), getHeight());
    alertWindow.addCustomComponent(fnc.get());
    alertWindow.addButton("Confirm", 1, KeyPress(KeyPress::returnKey));
    alertWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));
    
    
    if(alertWindow.runModalLoop())
    {
        _processor->_presetManager->loadPresetFromFile(fnc->getCurrentFile());
        updatePresetComboBox();
    }
}
