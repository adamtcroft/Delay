/*
  ==============================================================================

    PresetManager.h
    Created: 6 Oct 2020 10:21:14pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "InterfaceDefines.h"

class PresetManager
{
public:
    PresetManager(AudioProcessor* inProcessor);
    ~PresetManager();
    
    void getXmlForPreset(XmlElement* inElement);
    void loadPresetForXml(XmlElement* inElement);
    void createNewPreset();
    void savePreset();
    void saveAsPreset(String inPresetName, String storageDirectory);
    void loadPreset(int inPresetIndex);
    bool getIsCurrentPresetSaved();
    String getCurrentPresetName();
    int getNumberofPresets();
    String getPresetName(int inPresetIndex);
    void removePreset();
    int getCurrentPresetIndex();
    void loadStateOnOpen();
    void captureStateOnClose();
    void loadPresetFromFile(File file);
    
    File _currentlyLoadedPreset;
    bool _dontBotherUntilNextVersion {false};
    
private:
    void generatePresetList();
    
    AudioProcessor* _processor;
    XmlElement* _currentPresetXml;
    bool _currentPresetIsSaved;
    String _currentPresetName;
    String _presetDirectory;
    int _currentlyLoadedPresetIndex;
    Array<File> _localPresets;
    Array<String> _presetDirectories;
    
    const String SAVED_SETTINGS_FILE_LOCATION = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator + "NIR Plugin Presets" + directorySeparator + _processor->getName() + "Presets" + directorySeparator + "saveState.nirs";
    const String PRESETS_DEFAULT_DIRECTORY = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator +
    _processor->getName() + "Presets" + directorySeparator;

};
