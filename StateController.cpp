/*
  ==============================================================================

    StateController.cpp
    Created: 4 Oct 2020 1:28:14pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#include "StateController.h"

StateController::StateController(AudioProcessor& processor,
                                 UndoManager* undoManagerToUse,
                                 const Identifier& valueTreeType)
:   _apvts(processor, undoManagerToUse, valueTreeType, initializeParameters())
{
    
}

AudioProcessorValueTreeState::ParameterLayout StateController::initializeParameters()
{
    AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>("feedbackID",
                                                                         "Feedback",
                                                                         "Feedback",
                                                                         NormalisableRange<float>(0.0f, 0.95f),
                                                                         0.5f,
                                                                         nullptr,
                                                                         nullptr));
    
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>("delayTimeID",
                                                                         "Delay Time",
                                                                         "Delay Time",
                                                                         NormalisableRange<float>(0.01f, 5.f),
                                                                         3.f,
                                                                         nullptr,
                                                                         nullptr));
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("delayMovementID",
                "Delay Time Movement",
                "Delay Time Movement",
                NormalisableRange<float>(0.f, 1.f),
                0.f,
                nullptr,
                nullptr));
    layout.add(std::make_unique<AudioParameterFloat>
               ("delayMinID",
                "Delay Time Minimum",
                NormalisableRange<float>(0.01f, 5.f),
                0.01f));
    layout.add(std::make_unique<AudioParameterFloat>
               ("delayMaxID",
                "Delay Time Maximum",
                NormalisableRange<float>(0.01f, 5.f),
                5.f));
    
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("freeTimeButtonID",
                "Free Time Button",
                "FREE",
                NormalisableRange<float>(0.f, 1.f),
                1.f,
                nullptr,
                nullptr,
                false,
                true,
                false,
                AudioProcessorParameter::Category::genericParameter,
                true));
    
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("wetDryID",
                "Wet/Dry",
                "Wet/Dry",
                NormalisableRange<float>(0.f, 1.f),
                0.5f,
                nullptr,
                nullptr));
    
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("bitReductionID",
                "Bit Reduction",
                "Bit Reduction",
                NormalisableRange<float>(2.f, 24.f),
                24.f,
                nullptr,
                nullptr));
    
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("rateReductionID",
                "Sample Rate Reduction",
                "Rate Reduction",
                NormalisableRange<float>(1.f, 50.f),
                50.f,
                nullptr,
                nullptr));
    layout.add(std::make_unique<AudioProcessorValueTreeState::Parameter>
               ("rateMovementID",
                "Sample Rate Movement",
                "Sample Rate Movement",
                NormalisableRange<float>(0.f, 1.f),
                0.f,
                nullptr,
                nullptr));
    layout.add(std::make_unique<AudioParameterFloat>
               ("rateMinID",
                "Rate Reduction Minimum",
                NormalisableRange<float>(1.f, 50.f),
                1.f));
    layout.add(std::make_unique<AudioParameterFloat>
               ("rateMaxID",
                "Rate Reduction Maximum",
                NormalisableRange<float>(1.f, 50.f),
                50.f));
    
    return layout;
}
