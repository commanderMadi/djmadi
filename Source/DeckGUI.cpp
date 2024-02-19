/*
  ==============================================================================

    DeckGUI.cpp
    Created: 3 Feb 2024 12:00:46am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(juce::Colour &colorToUse, DJAudioPlayer* _djAudioPlayer, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) : djAudioPlayer(_djAudioPlayer), waveFormDisplay(colorToUse, formatManagerToUse, cacheToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(gainSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(waveFormDisplay);

    playButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    
    playButton.addListener(this);
    stopButton.addListener(this);
    
    gainSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    
    gainSlider.setRange(0,1);
    speedSlider.setRange(0,10);
    posSlider.setRange(0,1);
    
    startTimer(500);
}

DeckGUI::~DeckGUI() {
    stopTimer();
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
    }
}
void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        std::cout << "Gain Slider Value Updated: " << slider->getValue() << std::endl;
        double gain = gainSlider.getValue();
        djAudioPlayer->setGain(gain);
    }

    if (slider == &speedSlider) {
        std::cout << "Speed Slider Value Updated: " << slider->getValue() << std::endl;
        double ratio = speedSlider.getValue();
        djAudioPlayer->setSpeed(ratio);
    }

    if (slider == &posSlider) {
        std::cout << "Position Slider Value Updated: " << slider->getValue() << std::endl;
        double posInSecs = posSlider.getValue();
        djAudioPlayer->setRelativePosition(posInSecs);
    }
}
void DeckGUI::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colour(42, 46, 51));

}

void DeckGUI::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
//    float rowW = getWidth() / 4;
    float rowH = getHeight() / 4;
    waveFormDisplay.setBounds(0, 0, getWidth(), rowH);
//    playButton.setBounds(0, 0, getWidth(), rowH);
//    stopButton.setBounds(0, rowH, getWidth(), rowH);
//    gainSlider.setBounds(0, rowH*2, getWidth(), rowH);
//    posSlider.setBounds(0, rowH*3, getWidth(), rowH);
//    speedSlider.setBounds(0, rowH*4, getWidth(), rowH);
}


void DeckGUI::timerCallback() {
    waveFormDisplay.setRelativePosition(djAudioPlayer->getRelativePosition());
}

void DeckGUI::loadFileIntoDeck(const juce::String& trackURL, int deckId) {
    // Load the file into the appropriate deck based on the deckId
    juce::File file(trackURL);

    if (deckId == 1) {
        // Load into Deck 1
        // Implement your logic here
        std::cout << "Loading into Deck 1: " << trackURL << std::endl;
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
    } else if (deckId == 2) {
        // Load into Deck 2
        // Implement your logic here
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
        std::cout << "Loading into Deck 2: " << trackURL << std::endl;
    }
    // Add more conditions if you have additional decks
}
