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
                public juce::FileDragAndDropTarget,
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
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

private:
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::FileChooser fChooser{"Select a file..."};
    
    DJAudioPlayer* djAudioPlayer;
    WaveFormDisplay waveFormDisplay;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
