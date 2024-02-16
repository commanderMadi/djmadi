/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Feb 2024 12:00:46am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveFormDisplay.h"
//==============================================================================
/*
*/

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::Timer
{
    
public:
    DeckGUI(DJAudioPlayer* _djAudioPlayer, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
    ~DeckGUI() override;

    // Component virtual pure funcs
    void paint (juce::Graphics&g) override;
    void resized() override;
    
    // Timer virtual pure funcs
    void timerCallback() override;
    
    /* Implement Button Listener */
    void buttonClicked(juce::Button *) override;
    void sliderValueChanged(juce::Slider *) override;
    
    void loadFileIntoDeck(const juce::String& trackURL, int deckId);

private:
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    DJAudioPlayer* djAudioPlayer;
    WaveFormDisplay waveFormDisplay;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
