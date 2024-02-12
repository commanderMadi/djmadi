/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Feb 2024 1:54:14am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addToPlaylistButton);
    
    addToPlaylistButton.setButtonText("Add to Playlist");
    addToPlaylistButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::cyan);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0,0, getWidth(), getHeight() / 1.5);
    addToPlaylistButton.setBounds(getWidth() - 200, (getHeight() / 2) * 1.5, 200, 50);
}

int PlaylistComponent::getNumRows() {
    return static_cast<int>(trackTitles.size());
}
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton("play");
            juce::String id{std::to_string(rowNumber)};
            
            btn->addListener(this);
            btn->setComponentID(id);
            existingComponentToUpdate = btn;

        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    if (button != &addToPlaylistButton) {
        int id = std::stoi(button->getComponentID().toStdString());
        std::cout << "PlaylistComponent::buttonClicked for the internal playlist button instance" << trackTitles[id] << std::endl;
    }
    if (button == &addToPlaylistButton) {
        std::cout << "PlaylistComponent::buttonClicked for the add to playlist button" << std::endl;
        // - configure the dialogue
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that
        // but it is necessary as of JUCE 6.1
        fChooser.launchAsync(fileChooserFlags,
        [this](const juce::FileChooser& chooser) {
            auto chosenFile = chooser.getResult();
        });
    }
}
void PlaylistComponent::loadFile (juce::File audioFile) {
}
