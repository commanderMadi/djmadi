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
#include "CustomDesign.h"
//==============================================================================
/*
*/

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::ComboBox::Listener,
                public juce::Timer
{
    
public:
    DeckGUI(juce::Colour &colorToUse, juce::String &waveFormDefaultMessage, DJAudioPlayer* _djAudioPlayer, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
    ~DeckGUI() override;

    // Component virtual pure funcs
    void paint (juce::Graphics&g) override;
    void resized() override;
    
    // Timer virtual pure funcs
    void timerCallback() override;
    
    /* Implement Button Listener */
    void buttonClicked(juce::Button *) override;
    void sliderValueChanged(juce::Slider *) override;
    
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    
    void loadFileIntoDeck(const juce::String& trackURL, int deckId);
    void updateNowPlayingLabel(const juce::String& trackTitle);
    
    void setLoopRegion(double loopStart, double loopEnd);



private:
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ToggleButton loopButton;
    juce::ComboBox loopDurations;
    double currentLoopDuration{1.0};
    
    bool isTrackPlaying = false;
    
    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    juce::Label nowPlayingLabel;
    juce::Label gainSliderLabel;
    juce::Label speedSliderLabel;
    juce::Label posSliderLabel;


    std::unique_ptr<CustomDesign> customPlayButtonColor;
    std::unique_ptr<CustomDesign> customStopButtonColor;

    std::unique_ptr<CustomDesign> customSliderBackgroundColor;
    
    

    DJAudioPlayer* djAudioPlayer;
    WaveFormDisplay waveFormDisplay;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
