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
WaveFormDisplay::WaveFormDisplay(juce::Colour &colorToUse, juce::String &waveFormDefaultMessage, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) : colorToUse(colorToUse), waveFormDefaultMessage(waveFormDefaultMessage), audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    audioThumb.addChangeListener(this);
}

WaveFormDisplay::~WaveFormDisplay()
{
}

void WaveFormDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll (juce::Colour(42, 46, 51));
    g.setColour (juce::Colours::grey);
    
    g.drawRect (getLocalBounds(), 1.5);   // draw an outline around the component

    g.setColour (colorToUse);
    
    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(),0,audioThumb.getTotalLength(),0,1);
        g.setColour(colorToUse);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    } else {
        g.setFont (14.0f);
        g.drawText (waveFormDefaultMessage, getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

}

void WaveFormDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

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
    std::cout << "Change Received!" << std::endl;
    repaint();
}
void WaveFormDisplay::setRelativePosition(double pos) {
    if (position != pos) {
        position = pos;
        repaint();
    }
    
}
