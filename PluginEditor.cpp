/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuspensionAudioProcessorEditor::SuspensionAudioProcessorEditor (SuspensionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
      
    _lookAndFeel = new Sus_LookAndFeel();
    setLookAndFeel(_lookAndFeel);
    getLookAndFeel().setDefaultSansSerifTypeface(oswaldRegular);
    getLookAndFeel().setColour(Slider::trackColourId, juce::Colours::black);
    
    if(checkIfVersionOutOfDate() == true)
        displayUpdatePopup();
        
    setResizable(true, true);
    setResizeLimits(PLUGIN_WIDTH, PLUGIN_HEIGHT, PLUGIN_WIDTH+110, PLUGIN_HEIGHT+144);
    setSize (PLUGIN_WIDTH, PLUGIN_HEIGHT);
    
    //REMOVE IN NEXT VERSION
    if(File(File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator +
    audioProcessor.getName() + "Presets" + directorySeparator).exists())
    {
        File oldDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + directorySeparator +
        audioProcessor.getName() + "Presets" + directorySeparator;
        
        oldDirectory.deleteRecursively();
    }

    _mainPanel = std::make_unique<MainPanel>(&audioProcessor, getLocalBounds());
    addAndMakeVisible(*_mainPanel);
}

SuspensionAudioProcessorEditor::~SuspensionAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    delete _lookAndFeel;
}

//==============================================================================
void SuspensionAudioProcessorEditor::paint (Graphics& g)
{
}

void SuspensionAudioProcessorEditor::resized()
{
    setSize(getWidth(), getHeight());
    if(_mainPanel)
        _mainPanel->resized();
}

bool SuspensionAudioProcessorEditor::checkIfVersionOutOfDate()
{
    URL versionURL("https://adamtcroft.com/pkg_downloads/_version.xml");
    auto response = versionURL.readEntireXmlStream(false);
    double version;
    
    if(response)
        version = response->getChildByName("Suspension")->getDoubleAttribute("version");
    else
        return false;
    
    if(audioProcessor._presetManager->_dontBotherUntilNextVersion == true)
    {
        if(VERSION_NUMBER + 1.0 <= version)
        {
            audioProcessor._presetManager->_dontBotherUntilNextVersion = false;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(VERSION_NUMBER < version)
            return true;
        else
            return false;
    }
}

void SuspensionAudioProcessorEditor::displayUpdatePopup()
{
    AlertWindow alertWindow ("Update?", "There is a new version of " + processor.getName() + "! Would you like to update?", AlertWindow::NoIcon);
    alertWindow.setLookAndFeel(&getLookAndFeel());

    alertWindow.addButton("Yes", 1, KeyPress(KeyPress::returnKey));
    alertWindow.addButton("No", 0, KeyPress(KeyPress::escapeKey));
    //alertWindow.addButton("Don't Ask For a While", 2);
    
    int response = alertWindow.runModalLoop();

    if(response == 1)
    {
        URL("https://adamtcroft.com/pkg_downloads/Suspension.pkg").launchInDefaultBrowser();
    }
    if(response == 2)
    {
        audioProcessor._presetManager->_dontBotherUntilNextVersion = true;
    }
}
