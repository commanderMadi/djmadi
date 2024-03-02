/*
  ==============================================================================

    CustomDesign.cpp
    Created: 21 Feb 2024 12:09:22am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include "CustomDesign.h"

CustomDesign::CustomDesign(juce::Colour _baseColor): baseColor(_baseColor) {}

CustomDesign::~CustomDesign() {}

void CustomDesign::drawButtonBackground(juce::Graphics& g, 
                                        juce::Button& button,
                                        const juce::Colour& backgroundColour,
                                        bool shouldDrawButtonAsHighlighted,
                                        bool shouldDrawButtonAsDown) {
    
    // Only apply the color within the buttons borders, essentially as a background color
    auto bounds = button.getLocalBounds().toFloat();

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted) {
        // On button press down, make it slightly darker
        g.setColour(baseColor.darker());
    }
    // Set the background color
    g.setColour(baseColor);
    g.fillRoundedRectangle(bounds, 5.0f);
}

void CustomDesign::drawRotarySlider(juce::Graphics& g,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    float sliderPos,
                                    float rotaryStartAngle,
                                    float rotaryEndAngle,
                                    juce::Slider& slider) {
    // Draw the track color
    g.setColour(juce::Colour(baseColor));
    g.fillEllipse(x, y, width, height);
    
    // Draw the default rotary slider (track and thumb)
    juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
}
