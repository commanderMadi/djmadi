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

    /**
     * Virtual pure function to customize the background of a JUCE button
     * @param g Graphics object
     * @param backgroundColour The background color of the button
     * @param shouldDrawButtonAsHighlighted A boolean that check if the drawing should happen when the button is highlighted
     * @param shouldDrawButtonAsDown A boolean that check if the drawing should happen when the button is pressed down
     */
    void drawButtonBackground(juce::Graphics& g,
                              juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    /**
     * Virtual pure function to customize the rotary slider
     * @param g Graphics object
     * @param x The starting position on the x-axis
     * @param y The starting position on the y-axis
     * @param width The slider width
     * @param height The slider height
     * @param sliderPos The slider position
     * @param rotaryStartAngle the angle at which the the rotary slider is initialized
     * @param rotaryStartAngle the angle at which the the rotary slider is finished drawing
     * @param slider The slider object
     */
    void drawRotarySlider(juce::Graphics& g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          float rotaryStartAngle,
                          float rotaryEndAngle,
                          juce::Slider& slider) override;
private:
    juce::Colour baseColor;
};


