/*
  ==============================================================================

    InterfaceDefines.h
    Created: 13 Sep 2020 8:58:43pm
    Author:  Adam T. Croft

  ==============================================================================
*/

#pragma once

#define VERSION_NUMBER 0.18

#if JUCE_WINDOWS
    static const juce::String directorySeparator = "\\";
#elif JUCE_MAC
    static const juce::String directorySeparator = "/";
#endif

//namespace Sizes { static const int pluginHeight {340};}

#define PLUGIN_HEIGHT   400
#define PLUGIN_WIDTH    500

#define NUMBER_OF_SLIDERS 6

#define SLIDER_X                5
#define SLIDER_HEIGHT           20
#define SLIDER_LABEL_HEIGHT     12

#define BUTTON_WIDTH        50
#define BUTTON_HEIGHT       20
#define BUTTON_FONT_HEIGHT  10.f
#define BUTTON_SPACE        5

#define COMBO_BOX_FONT_HEIGHT   11.f
#define COMBO_BOX_WIDTH         100

#define POPUP_MENU_FONT_HEIGHT  14.f

#define ALERT_WINDOW_TITLE_FONT_HEIGHT      16.f
#define ALERT_WINDOW_MESSAGE_FONT_HEIGHT    14.f
#define ALERT_WINDOW_BUTTON_HEIGHT          BUTTON_HEIGHT+4
#define ALERT_WINDOW_BUTTON_FONT_HEIGHT     BUTTON_FONT_HEIGHT+4

#define FEEDBACK_BUTTONS_HEIGHT     BUTTON_HEIGHT+8
#define FEEDBACK_BUTTONS_WIDTH      BUTTON_WIDTH+25

#define REMOVE_FOR_PRESETS_PANEL    30

#define PRESET_FILE_EXTENSION ".nir"

const Typeface::Ptr oswaldRegular = Typeface::createSystemTypefaceFor(BinaryData::OswaldRegular_ttf, BinaryData::OswaldRegular_ttfSize);
const Typeface::Ptr oswaldLight = Typeface::createSystemTypefaceFor(BinaryData::OswaldLight_ttf, BinaryData::OswaldLight_ttfSize);

const Colour suspensionGrey = Colour(42, 48, 51);
const Colour suspensionMaroon = Colour(152, 30, 50);
