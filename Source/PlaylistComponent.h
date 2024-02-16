/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Feb 2024 1:54:14am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <functional>
#include "DeckGUI.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::FileDragAndDropTarget

{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    
    // Button::Listener pure virtual function
    void buttonClicked(juce::Button* button) override;
      
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    // a callback function that will pass down the track URL and the deck ID from the playlist component down to the deck
     
    using LoadIntoDeckCallback = std::function<void(const juce::String&, int)>;

    // Setting up the callback function
    void setLoadIntoDeckCallback(LoadIntoDeckCallback callback);

private:
    juce::File storageDirectory{juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory)};
    juce::AudioFormatManager formatManager;
    juce::File playlistStorageFile;
    juce::var playlistTracks;
    juce::TableListBox tableComponent;
    std::vector<juce::String>trackTitles;
    std::vector<juce::String>trackDurations;
    juce::TextButton addToPlaylistButton;
    juce::FileChooser fChooser{"Select a file..."};
    LoadIntoDeckCallback loadIntoDeckCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
