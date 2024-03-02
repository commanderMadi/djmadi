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
    MainComponent();
    
    ~MainComponent() override;
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics &g) override;
    
    void resized() override;
    
     /**
      * Loads the file that is passed down from the playlist into the selected deck based on its id.
      * @param trackURL the track URL
      * @param deckId the chosen deck ID
      */
    void loadFileIntoDeck(const juce::String& trackURL, int deckId);
    

  private:
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    juce::Colour colorLeft = juce::Colour(247, 119, 24);
    juce::Colour colorRight = juce::Colour(4, 167, 223);
    juce::String waveFormMessageLeft = "Waiting to load into deck 1";
    juce::String waveFormMessageRight = "Waiting to load into deck 2";
    DeckGUI deck1{colorLeft, waveFormMessageLeft, &player1, formatManager, thumbCache};
    DeckGUI deck2{colorRight,waveFormMessageRight, &player2, formatManager, thumbCache};
    PlaylistComponent playlist;
    juce::MixerAudioSource mixerSource;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};
