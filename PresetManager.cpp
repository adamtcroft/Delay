/*
  ==============================================================================

    PresetManager.cpp
    Created: 6 Oct 2020 10:21:14pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "PresetManager.h"

PresetManager::PresetManager(AudioProcessor* inProcessor)
:   _processor(inProcessor),
    _currentPresetXml(nullptr),
    _currentPresetIsSaved(false),
    _currentPresetName("Untitled")
{
    if(!File(PRESETS_DEFAULT_DIRECTORY).exists())
    {
        File(PRESETS_DEFAULT_DIRECTORY).createDirectory();
    }
}

PresetManager::~PresetManager()
{
    captureStateOnClose();
}

void PresetManager::getXmlForPreset(XmlElement* inElement)
{
    auto& parameters = _processor->getParameters();
    
    for(auto& parameter : parameters)
    {
        inElement->setAttribute(static_cast<AudioProcessorParameterWithID*>(parameter)->paramID, parameter->getValue());
    }
}

void PresetManager::loadPresetForXml(XmlElement* inElement)
{
    _currentPresetXml = inElement;
    
    auto& parameters = _processor->getParameters();

    for(int i = 0; i < _currentPresetXml->getNumAttributes(); i++)
    {
        const juce::String parameterID = _currentPresetXml->getAttributeName(i);
        
        for(auto& parameter : parameters)
        {
            if(parameterID == static_cast<AudioProcessorParameterWithID*>(parameter)->paramID)
                parameter->setValueNotifyingHost(_currentPresetXml->getDoubleAttribute(parameterID));
        }
    }
}

void PresetManager::createNewPreset()
{
    auto& parameters = _processor->getParameters();
    
    for(auto& parameter : parameters)
    {
        parameter->setValueNotifyingHost(parameter->getDefaultValue());
    }
    
    _currentPresetIsSaved = false;
    _currentPresetName = "Untitled";
}

void PresetManager::savePreset()
{
    MemoryBlock destinationData;
    _processor->getStateInformation(destinationData);
        
    _currentPresetIsSaved = _currentlyLoadedPreset.replaceWithData(destinationData.getData(), destinationData.getSize());
}

void PresetManager::saveAsPreset(String inPresetName, String storageDirectory)
{
    if(std::to_string(storageDirectory.getLastCharacter()) != directorySeparator)
        storageDirectory = storageDirectory + directorySeparator;
    
    File presetFile = File(storageDirectory + directorySeparator + inPresetName + PRESET_FILE_EXTENSION);
    
    if(!presetFile.exists())
    {
        presetFile.create();
    }
    else
    {
        presetFile.deleteFile();
    }
    
    _presetDirectories.addIfNotAlreadyThere(storageDirectory);
    
    MemoryBlock destinationData;
    _processor->getStateInformation(destinationData);
    _currentPresetIsSaved = presetFile.appendData(destinationData.getData(), destinationData.getSize());
    _currentPresetName = inPresetName;
    generatePresetList();
}

void PresetManager::loadPreset(int inPresetIndex)
{
    _currentlyLoadedPreset = _localPresets[inPresetIndex];
    _currentlyLoadedPresetIndex = inPresetIndex;
    
    MemoryBlock presetBinary;
    
    if(_currentlyLoadedPreset.loadFileAsData(presetBinary))
    {
        _currentPresetIsSaved = true;
        _currentPresetName = getPresetName(inPresetIndex);
        _processor->setStateInformation(presetBinary.getData(), static_cast<int>(presetBinary.getSize()));
    }
}

bool PresetManager::getIsCurrentPresetSaved()
{
    return _currentPresetIsSaved;
}

String PresetManager::getCurrentPresetName()
{
    return _currentPresetName;
}

int PresetManager::getNumberofPresets()
{
    return _localPresets.size();
}

String PresetManager::getPresetName(int inPresetIndex)
{
    return _localPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::generatePresetList()
{
    _localPresets.clear();
    
    for(auto directory : _presetDirectories)
    {
        for(DirectoryEntry entry : RangedDirectoryIterator(File(directory), false))
        {
            File preset = entry.getFile();
            if(preset.getFileExtension() == ".nir")
                _localPresets.add(preset);
        }
    }
}

void PresetManager::removePreset()
{
    _currentlyLoadedPreset.deleteFile();
    
    if(_currentlyLoadedPresetIndex > 0)
    {
        loadPreset(_currentlyLoadedPresetIndex - 1);
        generatePresetList();
    }
    else if(_currentlyLoadedPresetIndex == 0 && _localPresets.size() > 1)
    {
        loadPreset(_currentlyLoadedPresetIndex + 1);
        generatePresetList();
    }
    else
    {
        generatePresetList();
        _currentPresetName = "Untitled";
    }
}

int PresetManager::getCurrentPresetIndex()
{
    return _currentlyLoadedPresetIndex;
}

void PresetManager::loadStateOnOpen()
{
    MemoryBlock sourceData;
    
    if(File(SAVED_SETTINGS_FILE_LOCATION).exists())
    {
        File(SAVED_SETTINGS_FILE_LOCATION).loadFileAsData(sourceData);
    }
    
    std::unique_ptr<XmlElement> XmlState = _processor->getXmlFromBinary(sourceData.getData(), static_cast<int>(sourceData.getSize()));
    
    if(XmlState)
    {
        if(XmlState->getChildByName("DontBotherUntilNextVersion"))
            _dontBotherUntilNextVersion = XmlState->getChildByName("DontBotherUntilNextVersion")->getBoolAttribute("Value");
        
        forEachXmlChildElementWithTagName(*XmlState, child, "Directory")
        {
            _presetDirectories.addIfNotAlreadyThere(child->getAllSubText());
        }
        
        int presetToLoad = XmlState->getChildByName("CurrentPreset")->getIntAttribute("Preset");
        
        generatePresetList();
        
        if(presetToLoad < 0 || presetToLoad > _localPresets.size())
            presetToLoad = 0;
        
        loadPreset(presetToLoad);
    }
}

void PresetManager::captureStateOnClose()
{
    if(!File(SAVED_SETTINGS_FILE_LOCATION).exists())
    {
        File(SAVED_SETTINGS_FILE_LOCATION).create();
    }
    
    int currentPresetIndex = getCurrentPresetIndex();
    
    MemoryBlock destinationData;
    
    XmlElement loadState("LoadState");
    
    // Safe because parent calls delete on all children
    XmlElement* currentPreset = new XmlElement("CurrentPreset");
    currentPreset->setAttribute("Preset", currentPresetIndex);
    loadState.addChildElement(currentPreset);
    
    Array<String> allPaths;
    
    for(File preset : _localPresets)
    {
        String path = preset.getFullPathName().substring(0, preset.getFullPathName().lastIndexOf(directorySeparator)+1);
        
        if(allPaths.addIfNotAlreadyThere(path))
        {
            XmlElement* presetDirectory = new XmlElement("Directory");
            
            presetDirectory->addTextElement(path);
            loadState.addChildElement(presetDirectory);
        }
    }
    
    XmlElement* dontBother = new XmlElement("DontBotherUntilNextVersion");
    dontBother->setAttribute("Value", _dontBotherUntilNextVersion);
    loadState.addChildElement(dontBother);

    _processor->copyXmlToBinary(loadState, destinationData);
    File(SAVED_SETTINGS_FILE_LOCATION).replaceWithData(destinationData.getData(), destinationData.getSize());
}

void PresetManager::loadPresetFromFile(File file)
{
    _localPresets.add(file);
    auto index = _localPresets.indexOf(file);
    
    loadPreset(index);
}
