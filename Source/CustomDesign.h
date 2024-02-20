/*
  ==============================================================================

    CustomButtonDesign.h
    Created: 21 Feb 2024 1:53:11am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomDesign : public juce::LookAndFeel_V4 {
public:
    CustomDesign(juce::Colour baseColor);
    ~CustomDesign() override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
private:
    juce::Colour baseColor;
};


