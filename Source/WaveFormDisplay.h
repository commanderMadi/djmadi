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
class WaveFormDisplay  : public juce::Component,
                         public juce::ChangeListener {
public:
    WaveFormDisplay(juce::Colour &colorToUse,
                    juce::String &waveFormDefaultMessage,
                    juce::AudioFormatManager &formatManagerToUse,
                    juce::AudioThumbnailCache &cacheToUse);
                             
    ~WaveFormDisplay() override;

    void paint (juce::Graphics&g) override;
                             
    void resized() override;
    
  /**
   * Loading an audio file into the application
   * @param file The track file existent on the system
   */
    void loadFile (juce::File file);
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    // Set the relative position of the playhead
    void setRelativePosition(double pos);
    
 /**
  * Set the start and end position of the loop to draw the region on the waveForm
  * @param loopStart The loop starting position
  * @param loopEnd The loop ending position
  */
    void setLoopRegion(double loopStart, double loopEnd);
    

private:
    juce::Colour colorToUse;
    juce::String waveFormDefaultMessage;
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    bool loopRegionEnabled;
    double loopRegionStart;
    double loopRegionEnd;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormDisplay)
};
