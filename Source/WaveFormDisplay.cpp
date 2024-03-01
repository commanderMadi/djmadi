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
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1);
        if (loopRegionEnabled) {
            DBG("YES I WILL REPAINT RIGHT AWAY!");


            int loopStartX = juce::roundToInt(loopRegionStart * getWidth());
            int loopWidth = juce::roundToInt((loopRegionEnd - loopRegionStart) * getWidth());
            juce::Rectangle<int> loopRegionRect(loopStartX, 0, loopWidth, getHeight());
            
            DBG("LOOP REGION START: " << loopStartX);
            DBG("LOOP REGION WIDTH: " << loopWidth);

            g.setColour(juce::Colours::white.withAlpha(0.8f));  // Adjust the color and alpha as needed

            g.fillRect(loopRegionRect);

            
        }
        g.setColour(juce::Colours::white); // or any color that stands out
        auto drawPosition = static_cast<int>(position * getWidth());
        g.drawLine(drawPosition, 0, drawPosition, getHeight(), 2); // Last parameter is the line thickness
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
    if (loopControlsUpdater != nullptr) {
        loopControlsUpdater(pos);
    }
    
}
void WaveFormDisplay::setLoopRegion(double start, double end) {
    loopRegionStart = start / audioThumb.getTotalLength();
    loopRegionEnd = end / audioThumb.getTotalLength();
    loopRegionEnabled = (start < end);

    repaint();
    DBG("REPAINTING WAVE FORM NOW");
}

void WaveFormDisplay::setLoopControlsUpdater(std::function<void(double)> updater) {
    loopControlsUpdater = updater;
}
