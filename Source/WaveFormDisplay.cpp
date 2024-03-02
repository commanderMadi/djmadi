/*
  ==============================================================================

    WaveFormDisplay.cpp
    Created: 6 Feb 2024 10:57:03pm
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveFormDisplay.h"

//==============================================================================
WaveFormDisplay::WaveFormDisplay(juce::Colour &colorToUse,
                                 juce::String &waveFormDefaultMessage,
                                 juce::AudioFormatManager &formatManagerToUse,
                                 juce::AudioThumbnailCache &cacheToUse)
                                 : colorToUse(colorToUse),
                                   waveFormDefaultMessage(waveFormDefaultMessage),
                                   audioThumb(1000, formatManagerToUse, cacheToUse),
                                   fileLoaded(false),
                                   position(0) {
 
    audioThumb.addChangeListener(this);
}

WaveFormDisplay::~WaveFormDisplay(){}

void WaveFormDisplay::paint (juce::Graphics& g) {

    g.fillAll (juce::Colour(42, 46, 51));
    g.setColour (juce::Colours::grey);
    
    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1.5);

    g.setColour (colorToUse);
    
    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1);
        
        // If the user enabled the looping option, draw the loop region on the wave form
        if (loopRegionEnabled) {
            int loopStartX = juce::roundToInt(loopRegionStart * getWidth());
            int loopWidth = juce::roundToInt((loopRegionEnd - loopRegionStart) * getWidth());
            juce::Rectangle<int> loopRegionRect(loopStartX, 0, loopWidth, getHeight());
            
            g.setColour(juce::Colours::greenyellow.withAlpha(0.8f));
            g.fillRect(loopRegionRect);
        }
        
        // Set the playhead shape and color
        g.setColour(juce::Colours::white);
        auto drawPosition = static_cast<int>(position * getWidth());
        g.drawLine(drawPosition, 0, drawPosition, getHeight(), 2);
    } else {
        // Add some text if there is no loaded file.
        g.setFont (14.0f);
        g.drawText (waveFormDefaultMessage, getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveFormDisplay::resized() {}

void WaveFormDisplay::loadFile (juce::File audioFile) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::FileInputSource(audioFile));
    if (fileLoaded) {
        std::cout << "WFD loadFile working properly" << std::endl;
    } else {
        std::cout << "WFD loadFile failed to work properly" << std::endl;
    }
}

void WaveFormDisplay::changeListenerCallback (juce::ChangeBroadcaster* source) {
    repaint();
}

void WaveFormDisplay::setRelativePosition(double pos) {
    if (position != pos) {
        position = pos;
        repaint();
    }
}

void WaveFormDisplay::setLoopRegion(double start, double end) {
    // To get the correct position of the loop start and end on the waveform, divide the position of each by the total length of the track
    loopRegionStart = start / audioThumb.getTotalLength();
    loopRegionEnd = end / audioThumb.getTotalLength();
    // As long as the start is less than the end, the loop region will be drawn consistently
    loopRegionEnabled = (start < end);
    repaint();
}
