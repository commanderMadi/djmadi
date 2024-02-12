/*
  ==============================================================================

    WaveFormDisplay.h
    Created: 6 Feb 2024 10:57:03pm
    Author:  Ahmed Magdy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveFormDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    WaveFormDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
    ~WaveFormDisplay() override;

    void paint (juce::Graphics&g) override;
    void resized() override;
    
    void loadFile (juce::File file);
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    // Set the relative position of the playhead
    void setRelativePosition(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormDisplay)
};
