/*
  ==============================================================================

    StateController.h
    Created: 4 Oct 2020 1:28:14pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

struct StateController
{
    StateController(AudioProcessor& processor,
                    UndoManager* undoManagerToUse,
                    const Identifier& valueTreeType);
    ~StateController() = default;
    
    AudioProcessorValueTreeState::ParameterLayout initializeParameters();
    AudioProcessorValueTreeState _apvts;
};
