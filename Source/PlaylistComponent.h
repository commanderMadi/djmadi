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

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener
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
    
    void loadFile (juce::File audioFile);
    
private:
    juce::TableListBox tableComponent;
    std::vector<std::string>trackTitles;
    juce::TextButton addToPlaylistButton;
    juce::FileChooser fChooser{"Select a file..."};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
