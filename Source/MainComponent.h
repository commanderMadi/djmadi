#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent {
  public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    // Load the file that is passed down from the playlist into the selected deck based on its id.
    void loadFileIntoDeck(const juce::String& trackURL, int deckId);
    

  private:
    //==============================================================================
    // Your private member variables go here...
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    DeckGUI deck1{&player1, formatManager, thumbCache};
    DeckGUI deck2{&player2, formatManager, thumbCache};
    PlaylistComponent playlist;
    
    juce::MixerAudioSource mixerSource;
    
    juce::AudioFormatManager formatManager;

    juce::AudioThumbnailCache thumbCache{100};
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);

};
