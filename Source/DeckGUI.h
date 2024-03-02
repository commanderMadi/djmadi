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
    DeckGUI(juce::Colour &colorToUse,
            juce::String &waveFormDefaultMessage,
            DJAudioPlayer* _djAudioPlayer,
            juce::AudioFormatManager &formatManagerToUse,
            juce::AudioThumbnailCache &cacheToUse);
    
    ~DeckGUI() override;

    // Component virtual pure funcs
    void paint (juce::Graphics&g) override;
    
    void resized() override;
    
    // Timer virtual pure funcs
    void timerCallback() override;
    
    /* Implement Button Listener */
    void buttonClicked(juce::Button *) override;
    
    void sliderValueChanged(juce::Slider *) override;
    
    /**
     * Virtual Pure function to listen for changes on the loops durations dropdown list
     * @param comboBoxThatHasChanged The dropdown menu list that is being listened on
     */
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    
    /**
     * Sends a track down to a deck based on the provided deck's id
     * @param trackURL The track to send to the deck
     * @param deckId The chosen deck ID
     */
    void loadFileIntoDeck(const juce::String& trackURL, int deckId);
    
    /**
     * Updates the now playing label on each deck
     * @param trackTitle The track title to update when it starts playing
     */
    void updateNowPlayingLabel(const juce::String& trackTitle);

    /**
     * Set the start and end position of the loop to draw the region on the waveForm
     * @param loopStart The loop starting position
     * @param loopEnd The loop ending position
     */
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
